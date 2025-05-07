#define _CRT_SECURE_NO_WARNINGS
#include "dlist.h"
#include <stdlib.h>
#include <string.h>

/// <summary>
/// Allocates memory to the list and returns the pointer to it
/// </summary>
/// <returns>ptr to doctor list</returns>
docList* initDocList()
{
	docList* list = (docList*)malloc(sizeof(docList));
	if (list == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	list->head = EMPTY_LIST; //Initializes the head to NULL
	return list;
}

/// <summary>
/// Returns a pointer to the doctor that is first in the list
/// </summary>
/// <param name="list">ptr to list</param>
/// <returns>the fisrt doctor in the list</returns>
Doc* peekDocList(const docList* list)
{
	return list->head->doctor;
}

/// <summary>
/// Adds a new doctor to the top of the list
/// </summary>
/// <param name="list">ptr to list</param>
/// <param name="doctor">data of doctor to add</param>
void addToHeadDocList(docList* list, Doc doctor)
{
	Doc_node* temp = list->head;
	list->head = createDocNode(doctor); //Allocates memory to add a new node to add the doctor
	list->head->next = temp;
}

/// <summary>
/// Gets a list and prints the information about all the doctors
/// </summary>
/// <param name="list">ptr to list</param>
void printDocList(const docList* list)
{
	if (list->head == EMPTY_LIST) return;

	Doc_node* temp = list->head;
	while (temp != LIST_END) //for each doctor in the list
	{
		printDoc(temp->doctor);
		temp = temp->next;
	}
}

/// <summary>
/// Returns 1 if the list is empty and 0 if there are doctors in the list
/// </summary>
/// <param name="list">ptr to list</param>
/// <returns>1 for empty list or 0 for non empty</returns>
int isEmptyDocList(const docList* list)
{
	return (list->head == EMPTY_LIST);
}

/// <summary>
/// Goes through each node in the list and deletes all the dynamic memory assigned to it.
/// </summary>
/// <param name="list">ptr to list</param>
void destroyDocList(docList* list)
{
	if (isEmptyDocList(list)) //if the list is empty
	{
		free(list);
		return;
	}

	Doc_node* toDestroy = list->head;
	Doc_node* next;
	while (toDestroy != LIST_END) //while we are not in the end of the list
	{
		next = toDestroy->next;
		free(toDestroy->doctor->Name);
		free(toDestroy);
		toDestroy = next;
	}
	free(list);
}

/// <summary>
/// Creates a new node, allocates memory and enters the data into the new node
/// </summary>
/// <param name="doctor">new doctor data</param>
/// <returns>ptr to the new node</returns>
Doc_node* createDocNode(Doc doctor)
{
	//Allocates memory and checks its integrity:
	Doc_node* new_node = (Doc_node*)calloc(1, sizeof(Doc_node));
	if (new_node == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	new_node->doctor = (Doc*)calloc(1, sizeof(Doc));
	if (new_node->doctor == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	new_node->doctor->Name = calloc(30, sizeof(char));
	if (new_node->doctor->Name == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}

	//insert doctor data:
	strcpy(new_node->doctor->Name, doctor.Name); //insert the name doctor to the new node
	strcpy(new_node->doctor->nLicense, doctor.nLicense); //insert the nLicense to the new node
	new_node->doctor->nPatients = doctor.nPatients; //insert the nPatients to the new node
	new_node->next = NULL;
	return new_node;
}

/// <summary>
/// Gets a doctor's name and returns a pointer to the doctor structure
/// </summary>
/// <param name="list">ptr to list</param>
/// <param name="doctorName">doctor name to search in the list</param>
/// <returns>doctor struct</returns>
Doc* findDoctor(const docList* list, char* doctorName)
{
	Doc_node* temp = list->head;
	while (temp != LIST_END) //Go through each node in the list
	{
		if (strcmp(temp->doctor->Name, doctorName) == 0) return temp->doctor; //if the name is the same
		temp = temp->next;
	}
	printf("doctor not found"); //if not found
	return NULL;
}