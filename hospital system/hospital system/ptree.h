#pragma once

#ifndef _BST_HEADER__
#define _BST_HEADER__

#include "structure.h"

#define TREE_END NULL
#define EMPTY_TREE NULL

/*
typedef struct pInTree
{
	Patient* tpatient;
	struct pInTree* left;
	struct pInTree* right;
}pInTree;

typedef struct pTree
{
	pInTree* root;
}pTree;*/

//functions:

pTree* initPTree();
void insert2PTree(pTree* ptree, Patient tpatient);
void insertPTree(pInTree** root, Patient tpatient);
pInTree* createPInTreeNode(Patient tpatient);
void destroyPInTree(pInTree* root);
void destroyPTree(pTree* tree);
Patient* find(pTree* ptree, char* ID);
Patient* searchInTree(pInTree* root, char* ID);
int isPTreeEmpty(pTree* ptree);
int compareID(char* ID1, char* ID2);
void printAllPatient(pInTree* root);
void printAllPatientNameID(pInTree* root);
void removeFromPTree(pTree* ptree, char* ID);
pInTree* deleteNode(pInTree* root, char* ID);
pInTree* findMin(pInTree* node);


#endif
