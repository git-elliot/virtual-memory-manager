#include <stdio.h>
#define PAGEMASK 240
#define FRAMEMASK 15
int extractPageNumber(int num){
  return num & PAGEMASK;
}
int extractFrameNumber(int num){
  return num & FRAMEMASK;
}
int main(){
  int address;

  printf("Enter a address\n");
  scanf("%d",&address);
  printf("\nPage number : %d and Frame Number : %d",extractPageNumber(address),extractFrameNumber(address));
  return 0;
}
