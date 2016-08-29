/***
  Author: Atreya Madhav Joshi
  email: atreyamj@usc.edu
***/
#include <stdlib.h>
#include "my402list.h"

int My402ListEmpty(My402List* list){
  if(list==NULL){
    return TRUE;
  }
  if(list->num_members==0){
    return TRUE;
  }
  return FALSE;
}

int  My402ListLength(My402List* list){
  if(list == NULL){
    return 0;
  }
  return list->num_members;
}

My402ListElem *My402ListFind(My402List* list, void* obj){
  if(My402ListEmpty(list)){
    return NULL;
  }
  My402ListElem* elem = NULL;
  for(elem=My402ListFirst(list);elem!=NULL;elem=My402ListNext(list,elem)){
    if(elem->obj==obj){
      return elem;
    }
  }
  return NULL;

}
int My402ListInit(My402List* list){
  list->anchor.obj=NULL;
  list->anchor.next=&list->anchor;
  list->anchor.prev=&list->anchor;
  list->num_members=0;
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

My402ListElem *My402ListNext(My402List* list, My402ListElem* elem){
  if(My402ListEmpty(list)){
    return NULL;
  }
  if(elem==NULL){
    return NULL;
  }
  if(elem->next==&(list->anchor)){
    return NULL;
  }
  return elem->next;
}

My402ListElem *My402ListPrev(My402List* list, My402ListElem* elem){
  if(My402ListEmpty(list)){
    return NULL;
  }
  if(elem==NULL){
    return NULL;
  }
  if(elem->prev==&(list->anchor)){
    return NULL;
  }
  return elem->prev;
}

int  My402ListAppend(My402List* list, void* obj){
  My402ListElem* newNode=(My402ListElem *)malloc(sizeof(My402ListElem));
  if(newNode == NULL){
    return FALSE;
  }
  newNode->obj=obj;
  My402ListElem* lastNode=My402ListLast(list);
  if(My402ListEmpty(list)){
    if(!My402ListInit(list)){
      return FALSE;
    }
    newNode->prev=&(list->anchor);
    newNode->next=&(list->anchor);
    list->anchor.prev=newNode;
    list->anchor.next=newNode;
    list->num_members++;
    return TRUE;
  }
  newNode->prev=lastNode;
  lastNode->next=newNode;
  newNode->next=&(list->anchor);
  list->anchor.prev=newNode;
  list->num_members++;
  return TRUE;
}

int  My402ListPrepend(My402List* list, void* obj){
  My402ListElem* newNode=(My402ListElem *)malloc(sizeof(My402ListElem));
  if(newNode == NULL){
    return FALSE;
  }
  newNode->obj=obj;
  My402ListElem* firstNode=My402ListFirst(list);
  if(My402ListEmpty(list)){
    if(!My402ListInit(list)){
      return FALSE;
    }
    newNode->prev=&(list->anchor);
    newNode->next=&(list->anchor);
    list->anchor.prev=newNode;
    list->anchor.next=newNode;
    list->num_members++;
    return TRUE;
  }
  newNode->next=firstNode;
  firstNode->prev=newNode;
  newNode->prev=&(list->anchor);
  list->anchor.next=newNode;
  list->num_members++;
  return TRUE;
}

void My402ListUnlink(My402List* list, My402ListElem* elem){
  if(My402ListEmpty(list)){
    return;
  }
  if(elem==NULL){
    return;
  }
  if(elem->prev!=NULL){
    elem->prev->next=elem->next;
  }
  if(elem->next!=NULL){
    elem->next->prev=elem->prev;
  }
  list->num_members--;
}

void My402ListUnlinkAll(My402List* list){
  if(My402ListEmpty(list)){
    return;
  }
  My402ListElem* elem = NULL;
  for(elem=My402ListFirst(list);elem!=NULL;elem=My402ListNext(list,elem)){
    My402ListUnlink(list,elem);
  }
  My402ListInit(list);
}

int My402ListInsertAfter(My402List* list, void* obj, My402ListElem* elem){
  if(My402ListEmpty(list)){
    if(!My402ListInit(list)){
      return FALSE;
    }
  }
  if(elem==NULL){
    return My402ListAppend(list,obj);
  }
  My402ListElem* newNode=(My402ListElem *)malloc(sizeof(My402ListElem));
  if(newNode == NULL){
    return FALSE;
  }
  My402ListElem* nextNode=elem->next;
  newNode->obj=obj;
  newNode->prev=elem;
  newNode->next=nextNode;
  nextNode->prev=newNode;
  elem->next=newNode;
  list->num_members++;
  return TRUE;
}

int My402ListInsertBefore(My402List* list, void* obj, My402ListElem* elem){
  if(My402ListEmpty(list)){
    if(!My402ListInit(list)){
      return FALSE;
    }
  }
  if(elem==NULL){
    return My402ListPrepend(list,obj);
  }
  My402ListElem* newNode=(My402ListElem *)malloc(sizeof(My402ListElem));
  if(newNode == NULL){
    return FALSE;
  }
  My402ListElem* prevNode=elem->prev;
  newNode->obj=obj;
  newNode->prev=prevNode;
  newNode->next=elem;
  prevNode->next=newNode;
  elem->prev=newNode;
  list->num_members++;
  return TRUE;
}
