#include "terminal.h"
#include <string.h>


#define MAXAMOUNT 2000


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

	printf("Please Enter Transaction Date('DD/MM/YYYY'): ");
	gets(termData->transactionDate);
	if (strlen(termData->transactionDate) != 10 ||
		termData->transactionDate[2] != '/' ||
		termData->transactionDate[5] != '/' ||
		termData->transactionDate[0] < 48 ||
		termData->transactionDate[1] < 48 ||
		termData->transactionDate[3] < 48 ||
		termData->transactionDate[4] < 48 ||
		termData->transactionDate[6] < 48 ||
		termData->transactionDate[7] < 48 ||
		termData->transactionDate[8] < 48 ||
		termData->transactionDate[9] < 48 ||
		termData->transactionDate[0] > 57 ||
		termData->transactionDate[1] > 57 ||
		termData->transactionDate[3] > 57 ||
		termData->transactionDate[4] > 57 ||
		termData->transactionDate[6] > 57 ||
		termData->transactionDate[7] > 57 ||
		termData->transactionDate[8] > 57 ||
		termData->transactionDate[9] > 57) {
		printf("INVALID DATE FORMAT\n");
		return WRONG_DATE;
	}
	return OKAY;
}


EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	if (((termData.transactionDate[8] * 10) + termData.transactionDate[9]) < ((cardData.cardExpirationDate[3] * 10) + cardData.cardExpirationDate[4])) {
		return OK;
	}
	else if (((termData.transactionDate[8] * 10) + termData.transactionDate[9]) > ((cardData.cardExpirationDate[3] * 10) + cardData.cardExpirationDate[4])) {
		printf("Card is Expired\n");
		return EXPIRED_CARD;
	}
	else {
		if (((termData.transactionDate[3] * 10) + termData.transactionDate[4]) > ((cardData.cardExpirationDate[0] * 10) + cardData.cardExpirationDate[1])) {
			printf("Card is Expired\n");

			return EXPIRED_CARD;
		}
		else
			return OK;
	
	
	}



}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	float amount;
	printf("Enter Transaction Amount: ");
	scanf_s(" %f", &amount);
	
	if (amount <= 0) {
		printf("Invalid Amount\n");
		return INVALID_AMOUNT;

	}
	else {
		termData->transAmount = amount;
		return OK;
	
	}

}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount)
	{
		printf("Amount Exceeded The Max Amount\n");
		return EXCEED_MAX_AMOUNT;
	}
	else {
		return OK;
	}


}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	
	termData->maxTransAmount = MAXAMOUNT;
	if (termData->maxTransAmount <= 0) {
		printf("INVALID Max Amount\n");
		return INVALID_MAX_AMOUNT;

	}
	else
		return OK;

}

// for testing the functions above
/*int main() {



	ST_terminalData_t terminal;

	EN_terminalError_t result;
	

		result = setMaxAmount(&terminal);
		
		if (result == OK)
			printf("DONE\n");

	
	return 0;
}*/
