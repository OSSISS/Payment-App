/*
Terminla.c
*/

#include "STD_TYPES.h"
#include "card.h"
#include "terminal.h"
#include <stdio.h>
#include <string.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t Local_ErrorState = OK;
	uint32_t Local_StringLenght = 0;
	uint32_t Local_Month = 0, Local_Day = 0, Local_Year = 0;;;
	printf("Please, Enter the Transaction Date,\n");
	printf("On the following Format DD/MM/YYYY: ");
	gets(termData->transactionDate);
	(void)sscanf(termData->transactionDate, "%d/%d/%d", &Local_Day, &Local_Month , &Local_Year);
	Local_StringLenght = strlen(termData->transactionDate);
	if ((Local_StringLenght == 0) || (Local_StringLenght < 10) || (Local_Day > 31) || (Local_Day==0) || (Local_Month>12) || (Local_Month==0) || (Local_Year == 0) || (Local_Year<2022))
	{
		Local_ErrorState = WRONG_DATE;
	}
	return Local_ErrorState;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	EN_terminalError_t Local_ErrorState = OK;
	uint32_t Local_ExpirationYaer = 0;
	uint32_t Local_ExpirationMonth = 0;
	uint32_t Local_TransactionYaer = 0;
	uint32_t Local_TransactionMonth = 0;
	uint32_t Local_TransactionDay = 0;

	(void)sscanf(cardData.cardExpirationDate, "%d/%d", &Local_ExpirationMonth, &Local_ExpirationYaer);
	(void)sscanf(termData.transactionDate, "%d/%d/%d", &Local_TransactionDay, &Local_TransactionMonth, &Local_TransactionYaer);
	Local_TransactionYaer = Local_TransactionYaer % 100;
	if (Local_TransactionYaer > Local_ExpirationYaer)
	{
		Local_ErrorState = EXPIRED_CARD;
	}
	else if (Local_TransactionYaer == Local_ExpirationYaer)
	{
		if (Local_TransactionMonth > Local_ExpirationMonth)
		{
			Local_ErrorState = EXPIRED_CARD;
		}
	}
	return Local_ErrorState;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	EN_terminalError_t Local_ErrorState = OK;
	uint8_t Local_NumOfDigits = strlen(cardData->primaryAccountNumber);
	uint16_t Local_sum = 0;
	uint8_t Local_isSecond = 0;

	for (sint8_t Local_LoopIterator = Local_NumOfDigits - 1; Local_LoopIterator >= 0; Local_LoopIterator--) {

		uint8_t local_CurrentDigit = cardData->primaryAccountNumber[Local_LoopIterator] - '0'; 
		if (Local_isSecond == 1)
		{
			local_CurrentDigit = local_CurrentDigit * 2;
		}
		// We add two digits to handle
		// cases that make two digits after
		// doubling
		//16 ->1+6 = 7
		Local_sum += local_CurrentDigit / 10;
		Local_sum += local_CurrentDigit % 10;

		Local_isSecond = !Local_isSecond;
	}
	// 14--> 1+4 = 5
	if (Local_sum % 10 != 0)
	{
		Local_ErrorState = INVALID_CARD;
	}
	return Local_ErrorState;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t Local_ErrorState = OK;
	printf("Please Enter your Transaction amount: ");
	(void)scanf("%f%*c", &termData->transAmount);
	if (termData->transAmount <= 0)
	{
		Local_ErrorState = INVALID_AMOUNT;
	}
	return Local_ErrorState;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t Local_ErrorState = OK;
	
	if (termData->transAmount > termData->maxTransAmount)
	{
		Local_ErrorState = EXCEED_MAX_AMOUNT;
	}
	return Local_ErrorState;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t Local_ErrorState = OK;
	printf("Please Set The Max Allowable Amount: ");
	(void)scanf("%f%*c", &termData->maxTransAmount); // the format "%f%*c" not to use the enter Ascii in the next step
	if (termData->maxTransAmount <= 0)
	{
		Local_ErrorState = INVALID_MAX_AMOUNT;
	}
	return Local_ErrorState;
}
