/*
Server.c
*/

#include "STD_TYPES.h"
#include "card.h"
#include "terminal.h"
#include "server.h"
#include <string.h>

uint8_t Global_AccountIndexInDB = 0;
ST_accountsDB_t Global_ValidBankAccounts[MAX_ACCOUNTS_NUMBER] = {
																	{900000.0,	"5196081888500645"},
																	{22000.0,	"7992739871623067"},
																	{45500.0,   "3793545081623067"}
};

ST_transaction_t Global_ValidTransactionDB[MAX_ACCOUNTS_NUMBER] = { 0 };

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t Local_ErrorState = APPROVED;
	Local_ErrorState = isValidAccount(&transData->cardHolderData);
	if (Local_ErrorState == SOK)
	{
		Local_ErrorState = isAmountAvailable(&transData->terminalData);
		if (Local_ErrorState == SOK)
		{
			transData->transState = APPROVED;
			// Update Balance
			Global_ValidBankAccounts[Global_AccountIndexInDB].balance -= (transData->terminalData.transAmount);
			Local_ErrorState = saveTransaction(transData);
			if (Local_ErrorState !=SOK)
			{
				// error saving the data so we re add the amount to the balance
				Global_ValidBankAccounts[Global_AccountIndexInDB].balance += (transData->terminalData.transAmount);
			}
			else
			{
				Local_ErrorState = SOK;
			}

		}
		else
		{
			transData->transState = DECLINED_INSUFFECIENT_FUND;
			Local_ErrorState = saveTransaction(transData);
			if (Local_ErrorState == SOK)
			{
				Local_ErrorState = DECLINED_INSUFFECIENT_FUND;
			}
			else
			{
				Local_ErrorState = INTERNAL_SERVER_ERROR;
			}
		}
	}
	else
	{
		transData->transState = DECLINED_STOLEN_CARD;
		Local_ErrorState = saveTransaction(transData);
		if (Local_ErrorState == SOK)
		{
			Local_ErrorState = DECLINED_STOLEN_CARD;
		}
		else
		{
			Local_ErrorState = INTERNAL_SERVER_ERROR;
		}
	}
	return Local_ErrorState;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	EN_transState_t Local_ErrorState = OK;
	uint8_t Local_LoopIterator = 0;
	for (Local_LoopIterator = 0; Local_LoopIterator < MAX_ACCOUNTS_NUMBER; Local_LoopIterator++)
	{
		if (strcmp(cardData->primaryAccountNumber, Global_ValidBankAccounts[Local_LoopIterator].primaryAccountNumber) == 0)
		{
			Global_AccountIndexInDB = Local_LoopIterator;
			Local_ErrorState = OK;
			break;
		}
		else
		{
			Local_ErrorState = DECLINED_STOLEN_CARD;
			continue;
		}
	}
	
	return Local_ErrorState;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	EN_serverError_t Local_ErrorState = OK;
	if (termData->transAmount > Global_ValidBankAccounts[Global_AccountIndexInDB].balance)
	{
		Local_ErrorState = LOW_BALANCE;
	}
	return Local_ErrorState;
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t Local_ErrorState = OK;
	static uint8_t Local_TransactionDBIndex = 0;
	if (Local_TransactionDBIndex <= MAX_ACCOUNTS_NUMBER)
	{
		transData->transactionSequenceNumber = Local_TransactionDBIndex;
		memcpy(&Global_ValidTransactionDB[Local_TransactionDBIndex], transData, sizeof(ST_transaction_t));
		Local_TransactionDBIndex++;
	}
	else
	{
		Local_ErrorState = INTERNAL_SERVER_ERROR;
	}
	return Local_ErrorState;

}

