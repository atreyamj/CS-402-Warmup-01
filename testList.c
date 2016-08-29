#include <stdio.h>
#include <stdlib.h>
#include "my402list.h"

int main(){
  My402List list;
  My402ListInit(&list);
  int a=8;
  int b=9;
  int i=0;
  int* number=malloc(sizeof(int));
  for(i=1;i<=10;i++){

    *number=i;
    if(i==5){
     //My402ListAppend(&list,number);
     My402ListInsertAfter(&list,number,NULL);
    }
    else{
     My402ListInsertBefore(&list,number,NULL);
    }
    number=malloc(sizeof(int));
    //free(number);
  }
  number=malloc(sizeof(int));
  *number=100;
 // My402ListInsertBefore(&list,number,list.anchor.next->next);
  Print(&list);
  printf("\n%d\n",My402ListLength(&list));
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  My402ListUnlink(&list,list.anchor.next);
  printf("%d\n",My402ListLength(&list));
  printf("%d\n",*((int*)My402ListFind(&list,list.anchor.next->next->next->obj)->obj));
  My402ListUnlinkAll(&list);
  //printf(My402List)
  printf("%d\n",My402ListLength(&list));
  Print(&list);
  return 0;
}
