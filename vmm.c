#include <stdio.h>
#include <stdint.h>
#include<stdlib.h>

#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

int **physicalMemory;

int *pageTable;

int lastFrameNumber = -1;
FILE *logicalAddressStream;
FILE *backingStore;
int numberOfPageFaults=0;
unsigned short int extractPageNumber(uint16_t num){
  return (num & PAGEMASK) >> 8;
}
unsigned short int extractOffset(uint16_t num){
  return num & OFFSETMASK;
}

int retrieveFromStore(int pNo){
		
		int *buffer = (int*)malloc(256);

		fseek(backingStore,pNo*256,SEEK_SET);
		fread(buffer,1,8,backingStore);

		lastFrameNumber++;

		for(int i=0;i<256;i++){
			physicalMemory[lastFrameNumber][i] = buffer[i];
		}
		return lastFrameNumber;
}
int getFrame(int logicalAddress){
	int frameNumber = -1;
	int pNo = extractPageNumber(logicalAddress);
    frameNumber = pageTable[pNo];

	if(frameNumber== -1){
		numberOfPageFaults++;
		frameNumber = retrieveFromStore(pNo);
		pageTable[pNo] = frameNumber;
	}

	return frameNumber;
}

int main(){

  physicalMemory = (int **) malloc(sizeof(int *) * 256);
  
  pageTable = (int *) malloc(sizeof(256) * 256);

  uint16_t logicalAddress;
  
  int i=0;
  int frameNumber;
  uint16_t physicalAddress;
  uint16_t offset;

  for(int i=0;i<256;i++){
  	physicalMemory[i] = (int*)malloc(sizeof(int)*256);
  	pageTable[i] = -1;
  }
 
  backingStore = fopen("BackingStore.txt","rb");
  if(backingStore==NULL){
 	printf("Unable to load backingstore file\n");
	exit(1);
  }
  logicalAddressStream = fopen("address.txt","r");
  if(logicalAddressStream == NULL){
  	printf("Error opening the address file");
  	exit(1);
  }
  else{
  	i=0;
  	while(fscanf(logicalAddressStream,"%hx",&logicalAddress) != EOF && i<256){
  		  // printf("\n%dPage number : %x and Offset : %x",++i,extractPageNumber(logicalAddress),extractOffset(logicalAddress));
  		frameNumber = getFrame(logicalAddress);
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