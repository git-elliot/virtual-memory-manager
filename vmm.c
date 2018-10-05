#include <stdio.h>
#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF
unsigned int extractPageNumber(int num){
  return (num & PAGEMASK) >> 8;
}
unsigned int extractOffset(int num){
  return num & OFFSETMASK;
}
int main(){
  unsigned int address;
  printf("Enter a address\n");
  scanf("%x",&address);
  printf("\nPage number : %x and Offset : %x",extractPageNumber(address),extractOffset(address));
  return 0;
}
