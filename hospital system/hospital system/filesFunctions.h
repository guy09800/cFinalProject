#ifndef __FILE_FUNCTIONS_HEADER_
#define __FILE_FUNCTIONS_HEADER_

#include "structure.h"
#include "dlist.h"
#include "ptree.h"
#include "pline.h"
#include "pstack.h"


//functios:

void loadPatients(char* fileName, pTree* tree, docList* list);
void loadDoctors(char* fileName, docList* list);
void loadLine(char* fileName, pLine* line, pTree* tree);

void updateFiles(char* doctorFileName, char* lineFileName, char* patientFileName, pTree* tree, docList* list, pLine* line); //overall function to receive data from files
void updatePatients(char* fileName, pTree* tree);
void updateDoctors(char* fileName, docList* list);
void updateLine(char* fileName, pLine* line);

//Prints files in the same format that the functions receive the data:
void printPTreeToFile(pInTree* root, int* c, FILE* file);
void printAllergiesToFile(char allergies, FILE* file);
void printVstackToFile(Visit_node* visitNode, FILE* file);

#endif 