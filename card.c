/*
Card.c
*/

#include "STD_TYPES.h"
#include <stdio.h>
#include <string.h>
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) 
{
	EN_cardError_t Local_ErrorState = OK; // Local variable to return the error from the function
	uint8_t Local_StrLenght = 0; // local varialbe to carry the string lenght used to check the input errors
	printf("Please, Enter Your Card Holder Name : "); // Display a message on the console to guide the user
	gets(cardData->cardHolderName); // get string input form the console
	Local_StrLenght = strlen(cardData->cardHolderName); // calculate the length of the string
	if ((Local_StrLenght > 24) || (Local_StrLenght < 20) || (Local_StrLenght == 0))
	{
		Local_ErrorState = WRONG_NAME; // Wrong input format 
	}
	return Local_ErrorState; // return error state
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	EN_cardError_t Local_ErrorState = OK; // Local variable to return the error from the function
	uint8_t Local_StrLenght = 0;// local varialbe to carry the string lenght used to check the input errors.
	uint32_t Local_Month = 0, Local_Year = 0;
	printf("Please, Enter Your Card Expiration date, \n");// Display a message on the console to guide the user
	printf("On the following format \'MM/YY'\: ");
	gets(cardData->cardExpirationDate);// get string input form the console
	Local_StrLenght = strlen(cardData->cardExpirationDate); // calculate the length of the string
	(void)sscanf(cardData->cardExpirationDate, "%d/%d", &Local_Month, &Local_Year);
	if ((Local_StrLenght == 0) || (Local_StrLenght > 5) || (Local_StrLenght < 5))
	{
		Local_ErrorState = WRONG_EXP_DATE;// Wrong input format 
	}
	else if (Local_Month > 12 || Local_Month == 0 || Local_Year == 0)
	{
		Local_ErrorState = WRONG_EXP_DATE;// Wrong input format 
	}
	return Local_ErrorState;// return error state
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	EN_cardError_t Local_ErrorState = OK; // Local variable to return the error from the function
	uint8_t Local_StrLenght = 0; // local varialbe to carry the string lenght used to check the input errors
	printf("Please, Enter Your Card PAN: "); // Display a message on the console to guide the user
	gets(cardData->primaryAccountNumber); // get string input form the console
	Local_StrLenght = strlen(cardData->primaryAccountNumber);  // calculate the length of the string
	if ((Local_StrLenght == 0) || (Local_StrLenght > 19) || (Local_StrLenght < 10))
	{
		Local_ErrorState = WRONG_PAN; 
	}
	return Local_ErrorState;
}