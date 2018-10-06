#include <stdio.h>
#include <stdint.h>
#include<stdlib.h>

#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

int **physicalMemory;

int *pageTable;

int lastFrameNumber = -1;
<<<<<<< HEAD
FILE *logicalAddressStream;
FILE *backingStore;
int numberOfPageFaults=0;
=======

>>>>>>> ed4ab856863f8e5cd2364ad3dfee76d022c88834
unsigned short int extractPageNumber(uint16_t num){
  return (num & PAGEMASK) >> 8;
}
unsigned short int extractOffset(uint16_t num){
  return num & OFFSETMASK;
}

int retrieveFromStore(int pNo,FILE *backingStore){
		int *buffer = (int*)malloc(256);

		fseek(backingStore,pNo*256,SEEK_SET);
		fread(buffer,1,8,backingStore);

		lastFrameNumber++;

		for(int i=0;i<256;i++){
			physicalMemory[lastFrameNumber][i] = buffer[i];
		}
		return lastFrameNumber;
}
int getFrame(int logicalAddress,FILE* backingStore){
	int frameNumber = -1;
	int pNo = extractPageNumber(logicalAddress);
<<<<<<< HEAD
    frameNumber = pageTable[pNo];

	if(frameNumber== -1){
		numberOfPageFaults++;
		frameNumber = retrieveFromStore(pNo);
=======
	if(pageTable[pNo]!= -1){
		frameNumber = pageTable[pNo];
	}
	else{
		frameNumber = retrieveFromStore(pNo,backingStore);
>>>>>>> ed4ab856863f8e5cd2364ad3dfee76d022c88834
		pageTable[pNo] = frameNumber;
	}

	return frameNumber;
}

int main(){

  physicalMemory = (int **) malloc(sizeof(int *) * 256);
  
  pageTable = (int *) malloc(sizeof(256) * 256);

  uint16_t logicalAddress;
  FILE *logicalAddressStream;
  FILE *backingStore;
  int i=0;
  int frameNumber;
  uint16_t physicalAddress;
  uint16_t offset;

  for(int i=0;i<256;i++){
  	physicalMemory[i] = (int*)malloc(sizeof(int)*256);
  	pageTable[i] = -1;
  }

  logicalAddressStream = fopen("address.txt","r");
  backingStore = fopen("BackingStore.txt","rb");
  if(logicalAddressStream == NULL || backingStore == NULL){
  	printf("Error opening the address file/backing store");
  	exit(1);
  }
  else{
  	i=0;
  	while(fscanf(logicalAddressStream,"%hx",&logicalAddress) != EOF && i<256){
  		  // printf("\n%dPage number : %x and Offset : %x",++i,extractPageNumber(logicalAddress),extractOffset(logicalAddress));
  		frameNumber = getFrame(logicalAddress,backingStore);
  		offset = extractOffset(logicalAddress);
  		physicalAddress = (frameNumber << 8) + offset;
  		i++;
  		printf("%d> LA: %hx, PA: %hx, Data: %d\n",i,logicalAddress,physicalAddress,physicalMemory[frameNumber][offset]);
  	}
  }
  printf("Number of Page Faults : %d\n",numberOfPageFaults);
  fclose(logicalAddressStream);
  fclose(backingStore);
  return 0;
}