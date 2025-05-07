#pragma once

#ifndef __MY_LINE_HEADER_
#define __MY_LINE_HEADER_

#include "structure.h"
#include "ptree.h"

#define LINE_END NULL
#define EMPTY_LINE NULL

/*
typedef struct pInLine
{
	Patient* lpatient;
	struct pInLine* next;
}pInLine;

typedef struct pLine
{
	pInLine* head;
	pInLine* tail;
}pLine;
*/

//functions:

pLine* initLine();
Patient* removeFromHead(pLine* pline);
Patient* peekLine(const pLine* pline);
void addToTail(pLine* pline, Patient* lpatient);
void addToHead(pLine* pline, Patient* lpatient);
void printLine(const pLine* pline);
int isEmptyLine(const pLine* pline);
void destroyLine(pLine* pline);
pInLine* createpInLine(Patient* patient);
pInLine* findInLine(pLine* pline, char* ID);
void printPatientInLineNameID(pLine* pline);


#endif