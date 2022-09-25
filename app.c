/*
App.c
*/
#include <stdio.h>
#include <string.h>
#include "STD_TYPES.h"
#include "app.h"
#include "card.h"
#include "terminal.h"
#include "server.h"


void StartApp(void)
{
	ST_transaction_t ClientData;
	EN_cardError_t Local_CardErrorRetuen = OK;
	EN_terminalError_t Local_TerminalErrorReturn = TOK;
	EN_serverError_t Local_ServerErrorReturn = SOK;
	App_Steps_t Local_NextStep = INIT;
	printf("Welcom to our Payment Application\n");
	while (1)
	{
		switch (Local_NextStep)
		{
		case INIT: // Configure the maximum amount of the the system user can withdraw
			printf("\n");
			Local_TerminalErrorReturn = setMaxAmount(&ClientData.terminalData);
			printf("\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				printf("The Max Amount is saved \n");
				Local_NextStep = START;
			}
			else
			{
				printf("Wrong Max Amount Please, Try again \n");
			}
			break;
		case START:
			
			printf("\n");
			Local_CardErrorRetuen = getCardHolderName(&ClientData.cardHolderData);
			printf("\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = CARD_EXPIRATION;
			}
			else
			{
				printf("Wrong Card holder name. Please, Try again\n");
			}
			break;

		case CARD_EXPIRATION:
			printf("\n");
			Local_CardErrorRetuen = getCardExpiryDate(&ClientData.cardHolderData);
			printf("\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = CARD_PAN;
			}
			else
			{
				printf("Wrong Card Expiry Date\n");
				Local_NextStep = START;
			}
			break;
		case CARD_PAN:
			printf("\n");
			Local_CardErrorRetuen = getCardPAN(&ClientData.cardHolderData);
			printf("\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = IS_CARD_VALID;
			}
			else
			{
				printf("Wrong PAN format\n");
				Local_NextStep = START;
			}
			break;
		case IS_CARD_VALID:
			Local_TerminalErrorReturn = isValidCardPAN(&ClientData.cardHolderData);
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = GET_TRANSACTION_DATE;
			}
			else
			{
				printf("Your CARD IS NOT VALID\n");
				Local_NextStep = START;
			}
			break;
		case GET_TRANSACTION_DATE:
			printf("\n");
			Local_TerminalErrorReturn = getTransactionDate(&ClientData.terminalData);
			printf("\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = IS_CARD_EXPIRED;
			}
			else
			{
				printf("Wrong Transaction Date format \n");
				Local_NextStep = START;
			}
			break;
		case IS_CARD_EXPIRED:
			Local_TerminalErrorReturn = isCardExpired(ClientData.cardHolderData, ClientData.terminalData);
			printf("\n");
			if (Local_TerminalErrorReturn == OK)
			{
				Local_NextStep = GET_TRANSACTION_AMOUNT;
			}
			else
			{
				printf("Sorry Your Card Is Expired\n");
				Local_NextStep = START;
			}
			break;
		
		case GET_TRANSACTION_AMOUNT:
			printf("\n");
			Local_TerminalErrorReturn = getTransactionAmount(&ClientData.terminalData);
			printf("\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = IS_BELOW_MAX;
			}
			else
			{
				printf("error with Your Transaction amount \n");
				Local_NextStep = START;
			}
			break;
		case IS_BELOW_MAX:
			Local_TerminalErrorReturn = isBelowMaxAmount(&ClientData.terminalData);
			if (Local_TerminalErrorReturn == OK)
			{
				Local_NextStep = SNED_DATA_SERVER;
			}
			else
			{
				printf("Your Transaction Exceeds the max transaction amount\n");
				Local_NextStep = START;
			}
			break;
		case SNED_DATA_SERVER:
			ClientData.transactionSequenceNumber = 0;
			ClientData.transState = APPROVED;
			Local_ServerErrorReturn = recieveTransactionData(&ClientData);
			if (Local_ServerErrorReturn == SOK)
			{
				printf("Your Transaction is completed successfully\n");
			}
			else if (Local_ServerErrorReturn == DECLINED_INSUFFECIENT_FUND) {
				printf("Sorry, Your Transaction is Declined	 \n");
			
			}
			else if (Local_ServerErrorReturn == DECLINED_STOLEN_CARD)
			{
				printf("Sorry, Your PAN is not found	\n");
			}
			else
			{
				printf("Sorry, Your Transaction is Declined\n");
				printf("Please try again later\n");
			}
			Local_NextStep = START;
			break;
		}
	}
					
}
