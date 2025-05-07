#pragma once

#ifndef __MY_LIST_HEADER_
#define __MY_LIST_HEADER_

#include "structure.h"

#define LIST_END NULL
#define EMPTY_LIST NULL

/*
typedef struct Doc_node
{
	Doc* doctor;
	struct Doc_node* next;
}Doc_node;

typedef struct docList
{
	Doc_node* head;
}docList;
*/
//this list goes from head to the next antil NULL


//functios:

docList* initDocList();
Doc* peekDocList(const docList* list);
void addToHeadDocList(docList* list, Doc doctor);
void printDocList(const docList* list);
int isEmptyDocList(const docList* list);
void destroyDocList(docList* list);
Doc_node* createDocNode(Doc doctor);
Doc* findDoctor(const docList* list, char* doctorName);

#endif