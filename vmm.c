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

		int seekCheck = fseek(backingStore,pNo*256,SEEK_SET);
		if(seekCheck == 0){
			fread(buffer,1,256,backingStore);
			lastFrameNumber++;

			for(int i=0;i<256;i++){
				physicalMemory[lastFrameNumber][i] = buffer[i];
			}
			return lastFrameNumber;
		}
		else{
			printf("Seek error\n");
		}
		return -1;
}
int getFrame(int logicalAddress){
	int frameNumber = -1;
	int pNo = extractPageNumber(logicalAddress);
    frameNumber = pageTable[pNo];
 
	if(frameNumber == -1){
		numberOfPageFaults++;
		frameNumber = retrieveFromStore(pNo);
		pageTable[pNo] = frameNumber;
	}

	return frameNumber;
}

int main(){

  physicalMemory = (int **) malloc(sizeof(int *) * 256);
  
  pageTable = (int *) malloc(sizeof(int) * 256);

  uint16_t logicalAddress;
  int i=0;
  int frameNumber;
  uint16_t physicalAddress;
  uint16_t offset;

  for(int i=0;i<256;i++){
  	physicalMemory[i] = (int*)malloc(256);
  	pageTable[i] = -1;
  }

  logicalAddressStream = fopen("address.txt","r");
  backingStore = fopen("BackingStore.txt","rb");
  if(logicalAddressStream == NULL || backingStore == NULL){
  	printf("\nError opening the address file/backing store\n");
  	exit(1);
  }
  else{
  	i=0;
  	while(fscanf(logicalAddressStream,"%hx",&logicalAddress) != EOF && i<256){

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
