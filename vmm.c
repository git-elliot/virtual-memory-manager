#include <stdio.h>
#include <stdint.h>

#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF

int extractPageNumber(uint16_t num){
  return num & PAGEMASK;
}
int extractOffset(uint16_t num){
  return num & OFFSETMASK;
}
int main(){
  uint16_t address;

  printf("Enter a address\n");
  scanf("%hu\n",&address);
  printf("\nPage number : %hu and Offset : %hu",extractPageNumber(address),extractOffset(address));
  return 0;
}
