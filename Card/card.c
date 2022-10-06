/*
 * card.c
 *
 *  Created on: Sep 7, 2022
 *      Author: lenovo
 */


#include "card.h"
#include <string.h>


EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	printf("Card Holder Name: ");
	gets(cardData->cardHolderName);
	if (strlen(cardData->cardHolderName) > 25 || strlen(cardData->cardHolderName) < 20) {
		printf("INVALID NAME\n");
		return WRONG_NAME;
	}
	return OK;

}



EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	
	printf("Please Enter Expiry Date('MM/YY'): ");
	gets(cardData->cardExpirationDate);
	if (strlen(cardData->cardExpirationDate) != 5 ||
		cardData->cardExpirationDate[2] != '/' ||
		cardData->cardExpirationDate[0] < 48 ||
		cardData->cardExpirationDate[1] < 48 ||
		cardData->cardExpirationDate[3] < 48 ||
		cardData->cardExpirationDate[4] < 48 ||
		cardData->cardExpirationDate[0] > 57 ||
		cardData->cardExpirationDate[1] > 57 ||
		cardData->cardExpirationDate[3] > 57 ||
		cardData->cardExpirationDate[4] > 57)
	{
		printf("INVALID EXP DATE\n");
		return WRONG_EXP_DATE;
	}
	return OK;

}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Please Enter PAN: ");
	gets(cardData->primaryAccountNumber);
	if (strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) < 16) {
		printf("INVALID PAN\n");
		return WRONG_PAN;
	}
	for (uint8_t i = 0; i < strlen(cardData->primaryAccountNumber); i++) {
		if (cardData->primaryAccountNumber[i] < 48 || cardData->primaryAccountNumber[i]>57) {
			printf("INVALID PAN FORMAT");
			return WRONG_PAN;
		}
	}
	return OK;


}


// for testing the functions above
/*int main() {


	ST_cardData_t card;
	EN_cardError_t result;


	do {
		result = getCardPAN(&card);
		if (result == WRONG_PAN)
			printf("Try Again!\n");

	}while (result==WRONG_PAN);

	
}*/