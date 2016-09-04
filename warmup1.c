#include "cs402.h"
#include "my402list.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>




extern int errno;
typedef struct t_file_Transactions {
	bool isDeposit;
	time_t transactionTime;
	double transactionAmount;
	char *transactionDescription;
} TransactionRecord;


static char gszProgName[MAXPATHLENGTH];

int gnDebug=0;

int isDirectory(const char* fileName)
{
	if(fileName==NULL){
		return FALSE;
	}
	DIR* dirHandle = opendir(fileName);
	if(errno==ENOTDIR){
		return FALSE;
	}
	if(dirHandle != NULL)
	{
		closedir(dirHandle);
		return TRUE;
	}
	return FALSE;
}


void printUsage()
{
	fprintf(stderr,
		"usage: %s %s\n",
		gszProgName, "sort [tfile]");
		exit(-1);
	}

	FILE *getFileHandler(int argc, char* argv[])
	{
		if(argc!=3 && argc!=2){
			fprintf(stderr,"Invalid Option Provided\n");
			return NULL;
		}
		if(argv==NULL){
			fprintf(stderr,"No Options Provided\n");
			printUsage();
			return NULL;
		}
		if(!argv[0] ){
			fprintf(stderr,"Empty Options Provided\n");
			printUsage();
			return NULL;
		}

		if(strlen(argv[0])==0){
			fprintf(stderr,"Empty Options Provided\n");
			printUsage();
			return NULL;
		}
		if(strncmp(argv[1],"sort",4)!=0){
			fprintf(stderr,"Invalid Option Provided\n");
			printUsage();
			return NULL;
		}
		if(argv[2]){
			FILE* fileHandler=fopen(argv[2],"r");
			if(fileHandler==NULL){
				char* errorMessage= strerror(errno);
				if(errorMessage){
					fprintf(stderr,"Error Reading the File...\nError Detail(s): %s\n",errorMessage);
				}
				return NULL;
			}
			if(isDirectory(argv[2])){
				fprintf(stderr,"Error : Requested file is a Directory\n");
				return NULL;
			}
			return fileHandler;
		}
		else{
			return stdin;
		}

	}



	int isValidTransactionType(char* type,int count){
		if(type==NULL){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Type...\nError details: transaction type is NULL\n",count);
			return FALSE;
		}
		if(strlen(type)==0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Type...\nError details: transaction type is empty\n",count);
			return FALSE;
		}
		if(strncmp(type,"+",1)!=0 && strncmp(type,"-",1)!=0 ){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Type...\nError details: transaction type is non-standard value (+, - )\n",count);
			return FALSE;
		}
		return TRUE;
	}

	int isValidTimeStamp(char* timeStamp,int count){
		if(timeStamp==NULL){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time is NULL\n",count);
			return FALSE;
		}
		if(strlen(timeStamp)==0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time is empty\n",count);
			return FALSE;
		}
		if(strlen(timeStamp)>10){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time is too long\n",count);
			return FALSE;
		}
		if(atoi(timeStamp)<=0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time is missing or invalid (<= 0)\n",count);
			return FALSE;
		}
		if(atoi(timeStamp)-atof(timeStamp)!=0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time is missing or invalid\n",count);
			return FALSE;
		}
		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);
		if(currentTime.tv_sec < atoi(timeStamp)){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Time...\nError details: transaction time exceeds the current time\n",count);
			return FALSE;
		}
		return TRUE;
	}

	int isValidTransactionAmount(char* transactionAmount,int count){
		if(transactionAmount==NULL){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is NULL\n",count);
			return FALSE;
		}
		if(strlen(transactionAmount)==0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is empty\n",count);
			return FALSE;
		}
		if(!isdigit(transactionAmount[0])){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is missing\n",count);
			return FALSE;
		}
		char amount[strlen(transactionAmount)];
		strcpy(amount,transactionAmount);
		char* leftPart=strtok(amount,".");
		char* rightPart=strtok(NULL,".");
		if(strlen(leftPart)==strlen(transactionAmount) && strlen(leftPart)>7){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is too long\n",count);
			return FALSE;
		}
		if(rightPart==NULL){
			if(strlen(leftPart)>7 || atoi(leftPart)>10000000){
				fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is too long\n",count);
				return FALSE;
			}
			return TRUE;
		}
		if(strlen(leftPart)>7 || strlen(rightPart)>2){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is too long\n",count);
			return FALSE;
		}
		if(atoi(leftPart)>10000000){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is too long\n",count);
			return FALSE;
		}
		if(atoi(leftPart)<0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Amount...\nError details: transaction amount is too less( < 0 )\n",count);
			return FALSE;
		}
		return TRUE;

	}



	int isValidDescription(char* transactionDescription,int count){
		if(transactionDescription==NULL){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Description...\nError details: transaction description is NULL\n",count);
			return FALSE;
		}
		if(strlen(transactionDescription)==0 && isdigit(transactionDescription[0])){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction Description...\nError details: transaction description is EMPTY\n",count);
			return FALSE;
		}
		return TRUE;

	}


	char* trimDescription(char* transactionDescription){
		if(transactionDescription==NULL){
			return NULL;
		}
		while(isspace(*transactionDescription)){
			transactionDescription++;
		}
		return transactionDescription;
	}

	int isValidTransaction(char* transactionRecord,int count){
		if(transactionRecord==NULL){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction...\nError details: transaction is NULL\n",count);
			return FALSE;
		}
		if(strlen(transactionRecord)==0){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction...\nError details: transaction is empty\n",count);
			return FALSE;
		}
		if(strlen(transactionRecord)>1024){
			fprintf(stderr,"Error: [Line %d]: Invalid Transaction...\nError details: transaction is too lengthy\n",count);
			return FALSE;
		}
		char* transaction=(char*)(malloc(strlen(transactionRecord) * sizeof(char)));
		if(transaction==NULL){
			fprintf(stderr,"Error: Invalid Transaction...\nError details: transaction is NULL\n");
			return FALSE;
		}
		strncpy(transaction,transactionRecord,strlen(transactionRecord));
		char* type=strtok(transaction,"\t");
		char *timeStamp=strtok(NULL,"\t");
		char *transactionAmount=strtok(NULL,"\t");
		char *transactionDescription=strtok(NULL,"\t\n");
		transactionDescription=trimDescription(transactionDescription);
		if(!isValidTransactionType(type,count)){
			return FALSE;
		}
		if(!isValidTimeStamp(timeStamp,count)){
			return FALSE;
		}
		if(!isValidTransactionAmount(transactionAmount,count)){
			return FALSE;
		}
		if(!isValidDescription(transactionDescription,count)){
			return FALSE;
		}

		free(transaction);
		return TRUE;

	}

	int insertIntoList(My402List* list, TransactionRecord* transactionRecord ,int count){
		if(My402ListEmpty(list)){
			if(!My402ListInit(list)){
				fprintf(stderr,"Error: Transactions cannot be stored in list. List Cannot be initialised\n");
				return FALSE;
			}
			if(!My402ListPrepend(list,(void*)transactionRecord)){
				fprintf(stderr,"Error: [Line %d]: Storing Transaction failed\n",count);
				return FALSE;
			}
			return TRUE;
		}
		My402ListElem* elem = NULL;
		int i=1;
		for(elem=My402ListFirst(list);elem!=NULL;elem=My402ListNext(list,elem)){
			TransactionRecord *transaction = (TransactionRecord *) elem->obj;
			if(transaction->transactionTime==transactionRecord->transactionTime){
				fprintf(stderr,"Error: [Line %d and %d]: Duplicate Transaction. Found two transactions having same transaction time\n",count,i);
				return FALSE;
			}
			if(transaction->transactionTime>transactionRecord->transactionTime){
				if(!My402ListInsertBefore(list,transactionRecord,elem)){
					fprintf(stderr,"Error: [Line %d]: Storing Transaction failed\n",count);
					return FALSE;
				}
				return TRUE;
			}
			i++;
		}
		if(!My402ListInsertAfter(list,transactionRecord,elem)){
			fprintf(stderr,"Error: [Line %d]: Storing Transaction failed\n",count);
			return FALSE;
		}
		return TRUE;
	}

	int parseTransactions(FILE* fileHandler){
		if(fileHandler == NULL){
			return FALSE;
		}
		char* transaction=(char*)(malloc(1026*sizeof(char)));
		My402List list;
		if(!My402ListInit(&list)){
            fprintf(stderr,"Error Initializing the list..\n");
			return FALSE;
		}
		if(transaction==NULL){
			fprintf(stderr,"Error reading transactions...\nError details: Memory allocation failed while reading the transactions line by line\n");
			return FALSE;
		}
		int count=1;
		while((fgets(transaction,1026,fileHandler))!=NULL){
			TransactionRecord* transactionLine=(TransactionRecord*)(malloc(sizeof(TransactionRecord)));
			if(!transactionLine){
				fprintf(stderr,"Error reading transactions...\nError details: Memory allocation failed while reading the transactions line by line\n");
				return FALSE;
			}
			if(!isValidTransaction(transaction,count)){
				return FALSE;
			}
			char* type=strtok(transaction,"\t");
			char *timeStamp=strtok(NULL,"\t");
			char *transactionAmount=strtok(NULL,"\t");
			char *transactionDescription=strtok(NULL,"\t\n");
			if(strtok(NULL,"\t\n")){
				fprintf(stderr,"Error: [Line %d]: Too Many Fields.. \n",count);
				return FALSE;
			}
			if(!isValidTransactionType(type,count)){
				return FALSE;
			}
			if(strncmp(type,"+",1)==0){
				transactionLine->isDeposit=true;
			}
			else{
				transactionLine->isDeposit=false;
			}

			if(!isValidTimeStamp(timeStamp,count)){
				return FALSE;
			}
			transactionLine->transactionTime=atoi(timeStamp);

			if(!isValidTransactionAmount(transactionAmount,count)){
				return FALSE;
			}
			transactionLine->transactionAmount=atof(transactionAmount);

			transactionDescription=trimDescription(transactionDescription);
			if(!isValidDescription(transactionDescription,count)){
				return FALSE;
			}
			transactionLine->transactionDescription=(char*)(malloc(strlen(transactionDescription)));
			strcpy(transactionLine->transactionDescription,transactionDescription);
			if(!insertIntoList(&list,transactionLine,count)){
				return FALSE;
			}
			count++;
		}
		if(count==1){
			fprintf(stderr,"Error: Input is empty, no transaction data found\n");
			return TRUE;
		}
		return TRUE;
	}


	int test(int argc, char* argv[]){
		FILE *fileHandler=getFileHandler(argc, argv);
		if(fileHandler==NULL){
			fprintf(stderr,"Error occured during accessing transaction data\nExiting..\n");
			return -1;
		}
		if(!parseTransactions(fileHandler)){
            fprintf(stderr,"Exiting..\n");
            exit(-1);
		}
		return 0;
	}

	int main(int argc, char* argv[]){
		test(argc, argv);
		return 0;
	}
