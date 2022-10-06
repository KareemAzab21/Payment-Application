#include "application.h"
#include <stdlib.h>

extern ST_accountsDB_t acc_database[255];
extern ST_transaction_t trans_database[255];
extern uint8_t sequence;

typedef enum State { SUCCESS, FAIL }State;

static State SALE(ST_transaction_t* transData) {
	uint8_t i = getAccount(transData->cardHolderData);
	if (acc_database[i].balance == -1) {
		printf("Card is Blocked\n");
		return FAIL;
	
	}
	EN_transState_t result = recieveTransactionData(transData);
	if (result != APPROVED)
		return FAIL;
	else {
		saveTransaction(transData);
		return SUCCESS;
	}
		
	
}

void Block(ST_cardData_t* cardData) {
	uint8_t i = getAccount(*cardData);
	acc_database[i].balance = -1;
	
}


static State REFUND(ST_transaction_t* transData) {
	EN_serverError_t result = isValidAccount(&transData->cardHolderData);
	if (result == ACCOUNT_NOT_FOUND) {
		return FAIL;
			
	}
	else {
	
		if (transData->terminalData.transAmount > transData->terminalData.maxTransAmount) {
			printf("AMOUNT EXCEEDED MAXAMOUNT\n");
				return FAIL;
		}
		else {
			uint8_t i = getAccount(transData->cardHolderData);
			acc_database[i].balance += transData->terminalData.transAmount;
			sequence++;
			trans_database[sequence - 1] = *transData;
			trans_database[sequence - 1].transState = result;
			trans_database[sequence - 1].transactionSequenceNumber = sequence;
			return SUCCESS;
		
			}
	
	}

}

static State Pre_Authorization(ST_transaction_t* transData) {
	EN_transState_t result = recieveTransactionData(transData);
	if (result != APPROVED)
		return FAIL;
	else {
		return SUCCESS;
	}

}


static State VOID(ST_transaction_t* transData) {
	EN_serverError_t result = isValidAccount(&transData->cardHolderData);
	if (result == ACCOUNT_NOT_FOUND) {
		return FAIL;

	}
	else {

		if (transData->terminalData.transAmount > transData->terminalData.maxTransAmount) {
			printf("AMOUNT EXCEEDED MAXAMOUNT\n");
			return FAIL;
		}
		else {
			uint8_t i = getAccount(transData->cardHolderData);
			acc_database[i].balance += transData->terminalData.transAmount;
			trans_database[sequence - 1] = *transData;
			trans_database[sequence - 1].transState = result;
			trans_database[sequence - 1].transactionSequenceNumber = 0;
			sequence--;
			
			return SUCCESS;

		}

	}	

}



void appStart(void) {

	while (1) {
	

		ST_cardData_t cardData;
		ST_terminalData_t termData;
		EN_cardError_t valid;

		do {
			valid = getCardHolderName(&cardData);
			if (valid == OK) {
				valid = getCardExpiryDate(&cardData);
				if (valid == OK) {
					valid = getCardPAN(&cardData);
					if (valid != OK)
						printf("TRY AGAIN!\n");
				}
				else
					printf("TRY AGAIN!\n");

			}
			else {
				printf("TRY AGAIN!\n");
			}
		} while ((valid != OK));


		/* choose PAN from the current database to be valid
	{5000,"123123123123123123"},
	{100,"234234234234234234"},
	{50,"345345345345345345"},
	{2000,"456456456456456456"},
	{250,"567567567567567567"},
	{5,"678678678678678678"},
	{450,"123456789123456789"},
	{120,"23456789123456789"},
		 * */



		EN_terminalError_t result = getTransactionDate(&termData);
		if (result == OK) {
			result = isCardExpired(cardData, termData);
			if (result == OK) {
				result = getTransactionAmount(&termData);
				if (result == OK) {
					result = setMaxAmount(&termData);
				  result == isBelowMaxAmount(&termData);
				  if (result == EXCEED_MAX_AMOUNT) {
					  exit(EXIT_SUCCESS);
				  
				  }

				}
			}

		}
		if (result == OK) {
			uint8_t c;
			ST_transaction_t transData;
			transData.cardHolderData = cardData;
			transData.terminalData = termData;
			do {
				printf("a.SALE\nb.REFUND\nc.Pre_Authorization\nd.VOID\ne.Block Card\nf.QUIT\n");
				scanf_s(" %c", &c);
			} while (!(c >= 'a' && c <= 'e'));
			if (c == 'a') {
				State result = SALE(&transData);
				if (result == SUCCESS) {
					printf("SUCCESSFULL Transaction\n");
					exit(EXIT_SUCCESS);
				}
				else {
					printf("Transaction Declined\n");
					exit(EXIT_SUCCESS);
				}
			}
			else if (c == 'b') {
				State result = REFUND(&transData);
				if (result == SUCCESS) {
					printf("SUCCESSFULL Refund\n");
					exit(EXIT_SUCCESS);
				}
				else {
					printf("Transaction Declined\n");
					exit(EXIT_SUCCESS);
				}

			}
			else if (c == 'c') {
				State result = Pre_Authorization(&transData);
				if (result == SUCCESS) {
					printf("SUCCESSFULL Pre-Authorization\n");
					exit(EXIT_SUCCESS);
				}
				else {
					printf("Transaction Declined\n");
					exit(EXIT_SUCCESS);
				}


			}
			else if (c == 'd') {
				State result = VOID(&transData);
				if (result == SUCCESS) {
					printf("SUCCESSFULL Cancel Transaction\n");
					exit(EXIT_SUCCESS);
				}
				else {
					
					printf("Transaction Declined\n");
					exit(EXIT_SUCCESS);
				}

			}

			else if (c == 'e') {
			
			
				Block(&cardData);
			
			}

			else
				exit(EXIT_SUCCESS);

		}

		else
			continue;


	
	}
}

int main() {

	appStart();


}


