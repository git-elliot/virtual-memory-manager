#include <stdio.h>
#include <stdint.h>
#include<stdlib.h>

#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

int **physicalMemory;
int physicalMemory = (int**)malloc(sizeof(int*) * 256);
int physicalMemory[0] = (int*)malloc(sizeof(int) * 256 *256);
int *pageTable = (int*)malloc(sizeof(int) * 256);
uint16_t lastFrameNumber = -1;

unsigned short int extractPageNumber(uint16_t num){
  return (num & PAGEMASK) >> 8;
}
unsigned short int extractOffset(uint16_t num){
  return num & OFFSETMASK;
}
uint16_t getFrame(int logicalAddress){
	uint16_t frameNumber;
	uint16_t pNo = pageTable[extractPageNumber(logicalAddress)];
	if(pNo!= -1){
		frameNumber = pageTable[pNo];
	}
	else{
		frameNumber = retrieveFromStore(pNo);
	}
	return frameNumber;
}
uint16_t retrieveFromStore(uint16_t pNo){
		FILE *backingStore = fopen("backingStore.txt","rb");
		int *buffer = (int*)malloc(256);

		fseek(backingStore,pNo*256,SEEK_SET);
		fread(buffer,1,8,backingStore);

		for(int i=0;i<256;i++){
			physicalMemory[++lastFrameNumber][i] = buffer[i];
		}
		return lastFrameNumber;
}

int main(){
  uint16_t logicalAddress;
  FILE *logicalAddressStream;
  int i=0;
  uint16_t frameNumber;
  int physicalAddress;
  uint16_t offset;

  for(int i=0;i<256;i++){
  	pageTable[i] = -1;
  }

  logicalAddressStream = fopen("address.txt","r");
  if(logicalAddressStream == NULL){
  	printf("Error opening the address file");
  	exit(1);
  }
  else{
  	while(fscanf(logicalAddressStream,"%hx",&logicalAddress) != EOF){
  		  // printf("\n%dPage number : %x and Offset : %x",++i,extractPageNumber(logicalAddress),extractOffset(logicalAddress));
  		frameNumber = getFrame(logicalAddress);
  		offset = extractOffset(logicalAddress);
  		physicalAddress = (frameNumber >> 8) + offset;

  		printf("LA: %hx, PA: %hx, Data: %d\n",logicalAddress,physicalAddress,physicalMemory[frameNumber][offset]);
  	}
  }
  fclose(logicalAddressStream);
  return 0;
}