#include <stdio.h>
#define PAGEMASK 0xFF00
#define OFFSETMASK 0x00FF
int extractPageNumber(int num){
  return num & PAGEMASK;
}
int extractOffset(int num){
  return num & OFFSETMASK;
}
int main(){
  int address;

  printf("Enter a address\n");
  scanf("%x",&address);
  printf("\nPage number : %x and Offset : %x",extractPageNumber(address),extractOffset(address));
  return 0;
}
