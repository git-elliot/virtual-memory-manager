#include <stdio.h>
#include <stdint.h>
#include<stdlib.h>

#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

unsigned short int extractPageNumber(uint16_t num){
  return (num & PAGEMASK) >> 8;
}
unsigned short int extractOffset(uint16_t num){
  return num & OFFSETMASK;
}
int main(){
  uint16_t address;
  FILE *logicalAddressStream;
  int i=0;

  logicalAddressStream = fopen("address.txt","r");
  if(logicalAddressStream == NULL){
  	printf("Error opening the address file");
  	exit(1);
  }
  else{
  	while(fscanf(logicalAddressStream,"%x",&address) != EOF){
  		  printf("\n%dPage number : %x and Offset : %x",++i,extractPageNumber(address),extractOffset(address));
  	}
  }
  fclose(logicalAddressStream);
  return 0;
}