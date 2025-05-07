#pragma once
#ifndef __MY_STACK_HEADER_
#define __MY_STACK_HEADER_

#include "structure.h"

#define STACK_END NULL
#define EMPTY_STACK NULL

/*
typedef struct Visit_node
{
	Visit* visit;
	struct Visit_node* next;
}Visit_node;

typedef struct Visits
{
	Visit_node* head;
}Visits;
*/

//functions:

Visits* initVisitsStack();
void destroyVisitsStack(Visits* visits);
void pushToVisitsStack(Visits* visits, Visit visit);
Visit* popFromVisitsStack(Visits* visits);
Visit* peekVisitsStack(const Visits* visits);
int isEmptyVisitsStack(const Visits* visits);
void printVisitsStack(Visits* visits);
Visit_node* createVisit(Visit visit);

#endif