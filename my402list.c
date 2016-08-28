/***
  Author: Atreya Madhav Joshi
  email: atreyamj@usc.edu
***/
#include <stdlib.h>
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

My402ListElem* My402ListFirst(My402List* list){
  if(My402ListEmpty(list)){
    return NULL;
  }
  return list->anchor.next;
}

My402ListElem* My402ListLast(My402List* list){
  if(My402ListEmpty(list)){
    return NULL;
  }
  return list->anchor.prev;
}

My402ListElem *My402ListNext(My402List* list, My402ListElem* listElement){
  if(My402ListEmpty(list)){
    return NULL;
  }
  if(listElement==NULL){
    return NULL;
  }
  return listElement->next;
}

int  My402ListAppend(My402List* list, void* obj){
  My402ListElem* newNode=malloc(sizeof(My402ListElem));
  newNode->obj=obj;
  My402ListElem* lastNode=My402ListLast(list);
  if(My402ListEmpty(list)){
    newNode->prev=&list->anchor;
    newNode->next=&list->anchor;
    list->anchor.prev=newNode;
    list->anchor.next=newNode;
    return 0;
  }
  newNode->prev=lastNode;
  lastNode->next=newNode;
  newNode->next=&list->anchor;
  list->anchor.next=lastNode;
  lastNode->prev=&list->anchor;
  list->anchor.prev=newNode;
  return 0;
}
