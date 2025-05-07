#define _CRT_SECURE_NO_WARNINGS
#include <Stdlib.h>
#include <string.h>
#include "dlist.h"
#include "pstack.h"
#include "ptree.h"

/// <summary>
/// Creates a stack with dynamic memory and returns a pointer to the ADT
/// </summary>
/// <returns>ptr to stack ADT</returns>
Visits* initVisitsStack()
{
	Visits* visits = (Visits*)malloc(sizeof(Visits));
	if (visits == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	visits->head = EMPTY_STACK; //Initializes the stack head to NULL
	return visits;
}

/// <summary>
/// Receives a stack and deletes all the allocated dynamic memory
/// </summary>
/// <param name="visits">ptr to stack ADT</param>
void destroyVisitsStack(Visits* visits)
{
	Visit_node* toDestroy = visits->head;
	Visit_node* next;
	while (toDestroy != STACK_END)  //For each NODE in the stack
	{
		next = toDestroy->next;
		//free memory:
		free(toDestroy->visit->vSummary);
		free(toDestroy->visit);
		free(toDestroy); 
		toDestroy = next;
	}
	free(visits); //free visit stack ADT
}

/// <summary>
/// Receives a visit type structure and adds the data of the visit to the upper new node of the stack
/// </summary>
/// <param name="visits">ptr to stack ADT</param>
/// <param name="visit">struct - the visit data</param>
void pushToVisitsStack(Visits* visits, Visit visit)
{
	Visit_node* newHead = createVisit(visit); //create new list node
	newHead->next = visits->head; //push it to the stack
	visits->head = newHead;//update list head
}

/// <summary>
/// Removes the top visit from the top of the stack and returns a pointer to the visit
/// </summary>
/// <param name="visits">ptr to stack ADT</param>
/// <returns>ptr to visit</returns>
Visit* popFromVisitsStack(Visits* visits)
{
	if (isEmptyVisitsStack(visits)) return EMPTY_STACK; //if the stack is empty
	Visit* toReturn = visits->head->visit; //save the ptr to return
	Visit_node* tofree = visits->head; //save the node to free
	visits->head = visits->head->next; //set the new stack head
	free(tofree); //free the memory
	return toReturn;
}

/// <summary>
/// Returns the first visit to the stack
/// </summary>
/// <param name="visits">ptr to stack ADT</param>
/// <returns>ptr to visit struct</returns>
Visit* peekVisitsStack(const Visits* visits)
{
	if (visits->head == EMPTY_STACK) return EMPTY_STACK; //if there is no visits
	return visits->head->visit; //return the first visit
}

/// <summary>
/// Returns whether the stack is empty or not
/// </summary>
/// <param name="visits">ptr to stack ADT</param>
/// <returns>1 - empty, 0 - not empty</returns>
int isEmptyVisitsStack(const Visits* visits)
{
	return (visits->head == EMPTY_STACK);
}

/// <summary>
/// Goes through all the visits in the stack and prints the visits
/// </summary>
/// <param name="visits">ptr to visits stack ADT</param>
void printVisitsStack(Visits* visits)
{
	if (visits->head == EMPTY_STACK) //if there is no visits
	{
		printf("there are no visits\n");
		return;
	}

	Visit_node* temp = visits->head;
	while (temp != STACK_END) //for each visit in the stack
	{
		printVisit(temp->visit);
		temp = temp->next;
	}
}

/// <summary>
/// Receives the structure of a visit, creates a new NODE and puts all the data into it
/// </summary>
/// <param name="visit">visit struct data</param>
/// <returns>ptr to new visit node</returns>
Visit_node* createVisit(Visit visit)
{
	Visit_node* toReturn = (Visit_node*)calloc(1,sizeof(Visit_node)); //Allocates memory to the new NODE
	if (toReturn == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	toReturn->visit = calloc(1,sizeof(visit)); //Allocates memory for a visit inside the NODE
	if (toReturn->visit == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	//Initializes the data into the structure of the visit:
	toReturn->next = NULL;
	toReturn->visit->Doctor = visit.Doctor;
	toReturn->visit->tArrival = visit.tArrival;
	toReturn->visit->tDismissed = visit.tDismissed;
	toReturn->visit->Duration.hour = visit.Duration.hour;
	toReturn->visit->Duration.min = visit.Duration.min;
	toReturn->visit->vSummary = calloc(100, sizeof(char)); //Allocates memory for the visit summary
	if (toReturn->visit->vSummary == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	if (visit.vSummary != NULL) strcpy(toReturn->visit->vSummary, visit.vSummary);

	return toReturn;
}

	
