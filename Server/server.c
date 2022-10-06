#include "server.h"


 ST_accountsDB_t acc_database[255] = {
	{5000,"123123123123123123"},
	{100,"234234234234234234"},
	{50,"345345345345345345"},
	{2000,"456456456456456456"},
	{250,"567567567567567567"},
	{5,"678678678678678678"},
	{450,"123456789123456789"},
	{120,"23456789123456789"},
	 {0,"0"},
	 {0,"0"},
	 {0,"0"},

};
 ST_transaction_t trans_database[255] = { 0 };
  uint8_t sequence = 0;



 EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	 for (int i = 0; i < 20; i++) {
		 if (strcmp((uint8_t*)acc_database[i].primaryAccountNumber, (uint8_t*)cardData->primaryAccountNumber) == 0)
			 return SAFE;
	 }
	 printf("Invalid Account\n");
	 return DECLINED_STOLEN_CARD;
 
 }

 uint8_t getAccount(ST_cardData_t cardData) {
	 for (int i = 0; i < 20; i++) {
		 if (strcmp((uint8_t*)acc_database[i].primaryAccountNumber, (uint8_t*)cardData.primaryAccountNumber) == 0)
			 return i;
	 }
	 return -1;
	
 
 }




 EN_serverError_t isAmountAvailable(ST_cardData_t* cardData, ST_terminalData_t* termData) {
 
	uint8_t i= getAccount(*cardData);
	if (termData->transAmount > acc_database[i].balance) {
		printf("Insufficient Balance\n");
		return LOW_BALANCE;
	
	}
	return SAFE;
 }


 EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	 
	 uint8_t i = getAccount(transData->cardHolderData);
	 if (i == -1) {
		 printf("INVALID CARD\n");
		 return DECLINED_STOLEN_CARD;
	 }
	 else {
		 EN_serverError_t result = isAmountAvailable(&transData->cardHolderData, &transData->terminalData);
		 if (result == LOW_BALANCE) {
			 return DECLINED_INSUFFECIENT_FUND;
		 }
		 else {
			 int j;
			 for ( j= 0; j < 20; j++) {
				 if (strcmp(acc_database[j].primaryAccountNumber,"0")==0) {

					 acc_database[i].balance -= transData->terminalData.transAmount;
					 return APPROVED;
				 }

			 }

			 
			 

		 }
	 }
	 return INTERNAL_SERVER_ERROR;
 
 }





 EN_serverError_t saveTransaction(ST_transaction_t* transData) {
 
	 EN_transState_t result = recieveTransactionData(transData);
	 sequence++;
	 if (sequence > 255) {
		 return SAVING_FAILED;
	 }
	 else {

		 if (result == APPROVED) {
			 trans_database[sequence - 1] = *transData;
			 trans_database[sequence - 1].transState = result;
			 trans_database[sequence - 1].transactionSequenceNumber = sequence;

		 }
		 else if (result == DECLINED_INSUFFECIENT_FUND) {
			 trans_database[sequence - 1] = *transData;
			 trans_database[sequence - 1].transState = result;
			 trans_database[sequence - 1].transactionSequenceNumber = sequence;

		 }

		 else if (result == DECLINED_STOLEN_CARD) {
			 trans_database[sequence - 1] = *transData;
			 trans_database[sequence - 1].transState = result;
			 trans_database[sequence - 1].transactionSequenceNumber = sequence;

		 }
		 else {
			 trans_database[sequence - 1] = *transData;
			 trans_database[sequence - 1].transState = result;
			 trans_database[sequence - 1].transactionSequenceNumber = sequence;
		 }
	 
	 }
	 return SAFE;
	 
 }


 EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
 
	 if ( trans_database[transactionSequenceNumber-1].transactionSequenceNumber==transData->transactionSequenceNumber && (strcmp(trans_database[transactionSequenceNumber - 1].cardHolderData.primaryAccountNumber,transData->cardHolderData.primaryAccountNumber)==0)) {
		
		 return SAFE;
	 
	 }
	 else {
		 printf("Transaction Not Found");
		 return TRANSACTION_NOT_FOUND;
	 
	 
	 }
 
 }




 // for testing the functions above

 /*int main() {
	
	 ST_cardData_t card = { "Kareem Azab","123456789123456789","12/25" };
	 ST_terminalData_t terminal = { 200,5000,"22/12/2022" };
	 ST_transaction_t transaction = { card,terminal,APPROVED,1 };

	EN_transState_t result = saveTransaction(&transaction);
	 if (result == SAFE)
		 printf("DONE\n");
	 else {
		 printf("Declined!\n");

	 }
	 EN_transState_t result2 = getTransaction(1, &transaction);
	 if (result2 == SAFE)
		 printf("Transaction Found\n");

 
 
 }*/