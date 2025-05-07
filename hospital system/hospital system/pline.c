#define _CRT_SECURE_NO_WARNINGS
#include "pline.h"
#include "pstack.h"
#include <stdlib.h>

/// <summary>
/// Allocates memory to the line and returns the pointer to it
/// </summary>
/// <returns>ptr to line ADT</returns>
pLine* initLine()
{
	pLine* toreturn = (pLine*)malloc(sizeof(pLine));
	if (toreturn == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	toreturn->head = toreturn->tail = EMPTY_LINE; //Initializes the tail and head to NULL
	return toreturn;
}

/// <summary>
/// The function removes the node at the top of the queue and returns a pointer to the patient structure and deletes the dynamic memory of the node
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <returns>ptr to patient struct or NULL if empty</returns>
Patient* removeFromHead(pLine* pline)
{
	if (pline->head == EMPTY_LINE) return EMPTY_LINE;
	Patient* toReturn = pline->head->lpatient; //save return value
	pInLine* temp = pline->tail;
	pInLine* tofree = pline->head; //save ptr to free memory

	if (pline->tail == pline->head) //if only one in the line
	{
		pline->tail = EMPTY_LINE;
		free(pline->head);
		pline->head = EMPTY_LINE;
		return toReturn;
	}
	while (temp->next != pline->head) temp = temp->next; //go to second node from head

	pline->head = temp; //remove the first
	pline->head->next = LINE_END; //set the head ptr to be null
	free(tofree); //free the memory
	
	return toReturn;
}

/// <summary>
/// return ptr to the first patient in the line
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <returns>first in line ptr to patient</returns>
Patient* peekLine(const pLine* pline)
{
	return pline->head->lpatient;
}

/// <summary>
/// Gets a pointer to the patient and adds it to the end of the queue and allocates a suitable memory for it
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <param name="lpatient">ptr to patient struct</param>
void addToTail(pLine* pline, Patient* lpatient)
{
	pInLine* newTail = createpInLine(lpatient); //create new pInLine node
	if (pline->tail == EMPTY_LINE) 	//if this is first list element added to empty list
	{
		pline->head = pline->tail = newTail;//update list
		return;
	}
	else //list has at least one element
	{
		newTail->next = pline->tail;
		pline->tail = newTail;//update list tail
	}
}

/// <summary>
/// Gets a pointer to the patient and adds it to the first place(head) of the queue and allocates a suitable memory for it
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <param name="lpatient">ptr to patient struct</param>
void addToHead(pLine* pline, Patient* lpatient)
{
	pInLine* newHead = createpInLine(lpatient); //create new list node
	if (pline->tail == EMPTY_LINE) 	//if this is the first list element added to empty list
	{
		pline->head = pline->tail = newHead;//update list
		return;
	}
	else //list has at least one element
	{
		pline->head->next = newHead;
		pline->head = newHead;//update list
	}
}

/// <summary>
/// Goes through the entire list and prints the patients' names, ID's and allergies
/// </summary>
/// <param name="pline">ptr to line ADT</param>
void printLine(const pLine* pline)
{
	if (pline->head == EMPTY_LINE) return; //if empty line

	pInLine* temp = pline->tail;
	while (temp != LINE_END)
	{
		printPatient(temp->lpatient);
		temp = temp->next;
	}
	printf("end of line\n");
}

/// <summary>
/// Goes through the entire list and prints the patients' names and ID's
/// </summary>
/// <param name="pline">ptr to line ADT</param>
void printPatientInLineNameID(pLine* pline)
{
	pInLine* temp = pline->tail;
	printf("the line last place:\n");
	while (temp != pline->head) //From tail to head print the data
	{
		printPatientNameID(temp->lpatient);
		temp = temp->next;
	}
	printPatientNameID(temp->lpatient);
	printf("the line start\n");
}

/// <summary>
/// Checks if the queue is empty
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <returns> 1 - empty. 0 - not empty</returns> 
int isEmptyLine(const pLine* pline)
{
	return (pline->head == EMPTY_LINE && pline->tail == EMPTY_LINE);
}

/// <summary>
/// Receives a queue and deletes all the allocated dynamic memory
/// </summary>
/// <param name="pline">ptr to line ADT</param>
void destroyLine(pLine* pline)
{
	pInLine* toDestroy = pline->tail;
	pInLine* next;
	while (toDestroy != LINE_END) //For each NODE in the queue
	{
		next = toDestroy->next;
		free(toDestroy); //free the memory
		toDestroy = next;
	}
	free(pline); //free the ADT
}

/// <summary>
/// Receives a pointer to the patient and creates a new NODE for it, checks the allocated memory and inserts the pointer into it.
/// </summary>
/// <param name="patient">ptr to patient</param>
/// <returns>ptr to new pInLine</returns>
pInLine* createpInLine(Patient* patient)
{
	pInLine* toReturn = (pInLine*)malloc(sizeof(pInLine));
	if (toReturn == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}

	toReturn->lpatient = patient; //insert the pointer
	toReturn->next = NULL; //Initializes the next pointer
	return toReturn;
}

/// <summary>
/// Receives an ID card and returns the pointer to its position in the queue
/// </summary>
/// <param name="pline">ptr to line ADT</param>
/// <param name="ID"> to search by</param>
/// <returns>ptr to PInLine of this ID or NULL if not found</returns>
pInLine* findInLine(pLine* pline, char* ID)
{
	pInLine* temp = pline->tail;
	while (temp != LINE_END)
	{
		if (compareID(ID, temp->lpatient->ID) == 0) return temp; //if compareID return 0 so the ID is the same and return the pInLine
		temp = temp->next;
	}
	return NULL;
}
