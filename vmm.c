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

<<<<<<< HEAD
  printf("Enter a address\n");
  scanf("%hu",&address);
  printf("\nPage number : %hu and Offset : %hu",extractPageNumber(address),extractOffset(address));
=======
  logicalAddressStream = fopen("address.txt","r");
  if(logicalAddressStream == NULL){
  	printf("Error opening the address file");
  	exit(1);
  }
  else{
  	while(fscanf(logicalAddressStream,"%x",&address) != EOF){
  		  printf("\nPage number : %x and Offset : %x",extractPageNumber(address),extractOffset(address));
  	}
  }
  fclose(logicalAddressStream);
>>>>>>> 5bcdf9404e63b1385f8bb862f4b0ea1635906afd
  return 0;
}