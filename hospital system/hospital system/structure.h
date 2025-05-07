#pragma once

#ifndef __MY_STRUCTURE_HEADER_
#define __MY_STRUCTURE_HEADER_

#include <stdio.h>

//constants:
#define LICENSE_NUMBER 7
#define ID_LENGTH 10
#define NAME_LENGTH 50
#define SUMMARY_LENGTH 100
#define MIN_YEAR 1900
#define MAX_PATIENTS_PER_DOCTOR 4


typedef enum {
	none = 0b00000000, //0000 0000
	Penicillin = 0b00000001, // 0000 0001
	Sulfa = 0b00000010, // 0000 0010
	Opioids = 0b00000100, // 0000 0100
	Anesthetics = 0b00001000, // 0000 1000
	Eggs = 0b00010000, // 0001 0000
	Latex = 0b00100000, // 0010 0000
	Preservatives = 0b01000000, // 0100 0000
} Allergies;

typedef struct Date
{
	int Year; 
	int Month; 
	int Day; 
	int Hour; 
	int Min; 
}Date;

typedef struct Doc
{
	char* Name; //ptr to dynamic string (letters and spaces only) 
	char nLicense[LICENSE_NUMBER]; //only number, no doplicates
	int nPatients; //max = 4 (MAX_PATIENTS_PER_DOCTOR = 4)
}Doc;

typedef struct Visit
{
	Date tArrival; 
	Date tDismissed; //if still in the hospital all the field will be -1
	struct Duration
	{
		float hour;
		float min;
	}Duration;//if still in the hospital all the field will be -1
	Doc* Doctor; //ptr to doc struct
	char* vSummary; //ptr to dynamic string (if there isn't will be null
}Visit;

typedef struct Visit_node
{
	Visit* visit;
	struct Visit_node* next;
}Visit_node;

typedef struct Visits //will be as stack
{
	Visit_node* head;
}Visits;

typedef struct Patient
{
	char Name[NAME_LENGTH];
	char ID[ID_LENGTH];
	char Allergies;
	Visits* visits; //ptr to stack contain all patient visit
	int nVisits; //all the visits in the stack
}Patient;

//NODE's

typedef struct Doc_node
{
	Doc* doctor;
	struct Doc_node* next;
}Doc_node;

typedef struct pInTree
{
	Patient* tpatient;
	struct pInTree* left;
	struct pInTree* right;
}pInTree;

typedef struct pInLine
{
	Patient* lpatient;
	struct pInLine* next;
}pInLine;


//ADT's:

typedef struct pLine //will be as line
{
	pInLine* head;
	pInLine* tail;
}pLine;

typedef struct docList //will be as one way list
{
	Doc_node* head;
}docList;

typedef struct pTree //will be BST
{
	pInTree* root;
}pTree;

//printing functions:

void printDoc(const Doc* doctor);
void printAllergies(char allergies);
void printPatient(const Patient* patient);
void printPatientNameID(const Patient* patient);
void printVisit(const Visit* visit);
void printDate(Date date, int level);

#endif