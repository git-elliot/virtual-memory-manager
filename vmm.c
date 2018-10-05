#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

uint8_t **physicalMemory;

uint8_t *pageTable;
int numberOfPageFaults=0;
uint8_t lastFrameNumber = 0;

uint8_t extractPageNumber(uint16_t num){
  return (num & PAGEMASK) >> 8;
}
uint8_t extractOffset(uint16_t num){
  return num & OFFSETMASK;
}

uint8_t retrieveFromStore(uint8_t pNo){
	   
		FILE *backingStore = fopen("BackingStore.txt","rb");
		uint8_t *buffer = (uint8_t*)malloc(256);

		fseek(backingStore,pNo*256,SEEK_SET);
		fread(buffer,1,8,backingStore);

		for(int i=0;i<256;i++){
			physicalMemory[++lastFrameNumber][i] = buffer[i];
		}
		pageTable[pNo]= lastFrameNumber;
		fclose(backingStore);
		return lastFrameNumber;
}
uint8_t getFrame(uint16_t logicalAddress){
	uint8_t pNo = extractPageNumber(logicalAddress);
	uint8_t frameNumber = pageTable[pNo];
	
	if(frameNumber == 0){
		return retrieveFromStore(pNo); 
	}
	return frameNumber;
}

int main(){

  physicalMemory = (uint8_t **) malloc(sizeof(uint8_t *) * 256);
  

  pageTable = (uint8_t *) malloc(sizeof(uint8_t) * 256);
  if(pageTable == NULL){
  	printf("Out of memory for page table\n");
  	exit(1);
  }

  uint16_t logicalAddress;
  FILE *logicalAddressStream;
  int i=0;
  uint8_t frameNumber;
  uint16_t physicalAddress;
  uint8_t offset;

  for(int i=0;i<256;i++){
  	physicalMemory[i] = (uint8_t*)malloc(sizeof(uint8_t)*256);
    pageTable[i] =0;
  }
  

  logicalAddressStream = fopen("address.txt","r");
  if(logicalAddressStream == NULL){
  	printf("Error opening the address file");
  	exit(1);
  }
  else{
  	while(fscanf(logicalAddressStream,"%hu",&logicalAddress) != EOF){
  		  // printf("\n%dPage number : %x and Offset : %x",++i,extractPageNumber(logicalAddress),extractOffset(logicalAddress));
  		frameNumber = getFrame(logicalAddress);
  		offset = extractOffset(logicalAddress);
  		physicalAddress = (frameNumber >> 8) + offset;

  	//	printf("LA: %u, PA: %u, Data: %d\n",logicalAddress,physicalAddress,physicalMemory[frameNumber][offset]);
  	}
  }
  fclose(logicalAddressStream);
  printf("\nNo. of page faults occured : %d\n",numberOfPageFaults);
  return 0;
}