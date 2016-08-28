/***
  Author: Atreya Madhav Joshi
  email: atreyamj@usc.edu
***/

#include "my402list.h"

int My402ListEmpty(My402List* list){
  if(list == NULL){
    return TRUE;
  }
  if(list->anchor.next == NULL && list->anchor.prev == NULL){
    return TRUE;
  }
  return FALSE;
}

int My402ListInit(My402List* list){
 list->anchor.obj=NULL;
 list->anchor.next=NULL;
 list->anchor.prev=NULL;
 return TRUE;
}
