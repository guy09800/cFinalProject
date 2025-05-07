#define _CRT_SECURE_NO_WARNINGS
#include "ptree.h"
#include "pstack.h"
#include "pline.h"
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

/// <summary>
/// Allocates memory to the BST and returns the pointer to it
/// </summary>
/// <returns>ptr to BST ADT</returns>
pTree* initPTree()
{
	pTree* toRetrun = (pTree*)malloc(sizeof(pTree));
	if (toRetrun == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	toRetrun->root = NULL; //Initializes the root to NULL
	return toRetrun;
}

/// <summary>
/// Gets a pointer to the BST ADT and inserts a new patient structure in the right place in the tree
/// </summary>
/// <param name="ptree">ptr to BST ADT</param>
/// <param name="tpatient">patient struct data</param>
void insert2PTree(pTree* ptree, Patient tpatient)
{
	insertPTree(&ptree->root, tpatient); 
}

/// <summary>
/// given patient struct and add it to the tree in the right place
/// </summary>
/// <param name="root">ptr of the tree root</param>
/// <param name="tpatient">patient struct to add</param>
void insertPTree(pInTree** root, Patient tpatient)
{
	if (*root != EMPTY_TREE) //if not null so still need to find the right place
	{
		int compID = compareID((*root)->tpatient->ID, tpatient.ID); //find if the id is bigger or lower
		if (compID == 0) return;//no duplicates
		if (compID < 0)
		{
			if ((*root)->left == NULL)//no left child
			{
				(*root)->left = createPInTreeNode(tpatient); //create and insert the node
				return;
			}
			else insertPTree(&(*root)->left, tpatient); //keep search the right place
		}
		else
		{
			if ((*root)->right == NULL)//no right child
			{
				(*root)->right = createPInTreeNode(tpatient); //create and insert the node
				return;
			}
			else insertPTree(&(*root)->right, tpatient); //keep search the right place
		}
	}
	else //case the tree is empty
	{
		*root = createPInTreeNode(tpatient);//create and insert the node
	}
}

/// <summary>
/// create new node from the patient data and return the new node ptr
/// </summary>
/// <param name="tpatient">patient data</param>
/// <returns>the ptr for the new node</returns>
pInTree* createPInTreeNode(Patient tpatient)
{
	pInTree* newNode = (pInTree*)calloc(1, sizeof(pInTree)); // Allocate memory
	if (newNode == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	newNode->tpatient = (Patient*)calloc(1, sizeof(Patient));  // Allocate memory
	if (newNode->tpatient == NULL) {
		printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
		exit(1);
	}
	//insert the data into the new node:
	strcpy(newNode->tpatient->Name, tpatient.Name);
	strcpy(newNode->tpatient->ID, tpatient.ID);
	newNode->tpatient->Allergies = tpatient.Allergies;
	newNode->tpatient->visits = tpatient.visits;
	newNode->tpatient->nVisits = tpatient.nVisits;

	newNode->left = newNode->right = TREE_END; //sets the children to NULL
	return newNode;
}

/// <summary>
/// destroy the root and all his children and free the memory
/// </summary>
/// <param name="root">ptr to the root of the tree</param>
void destroyPInTree(pInTree* root)
{
	if (root == NULL) return;
	destroyPInTree(root->left); //go left
	destroyPInTree(root->right); //go right
	//free all the memory:
	destroyVisitsStack(root->tpatient->visits);
	free(root->tpatient);
	free(root);
}

/// <summary>
/// destroy the tree and free the memory
/// </summary>
/// <param name="tree">ptr to tree ADT</param>
void destroyPTree(pTree* tree)
{
	if (tree == EMPTY_TREE) return;
	destroyPInTree(tree->root);
	free(tree);
}

/// <summary>
/// find ID patient in the tree
/// </summary>
/// <param name="ptree">ptr to tree ADT</param>
/// <param name="ID">ptr to ID</param>
/// <returns>pointer to patient data in the tree node or NULL if not found</returns>
Patient* find(pTree* ptree, char* ID)
{
	return searchInTree(ptree->root, ID);
}

/// <summary>
/// find the ID in the tree
/// </summary>
/// <param name="root">ptr to the root of the tree</param>
/// <param name="ID">ptr to id we search</param>
/// <returns>pointer to patient data in the tree</returns>
Patient* searchInTree(pInTree* root, char* ID)
{
	if (root == TREE_END) return TREE_END; //if not found
	int compID = compareID(root->tpatient->ID, ID); //go left or right?
	if (compID == 0) return root->tpatient; //if we find the right patient
	if (compID > 0) return searchInTree(root->right, ID); //go right
	else return searchInTree(root->left, ID); //go left
}

/// <summary>
/// if the tree empty return 1 else return 0
/// </summary>
/// <param name="ptree">ptr to tree ADT</param>
/// <returns> 1 - empty tree, 0 - not empty</returns>
int isPTreeEmpty(pTree* ptree)
{
	return (ptree->root == EMPTY_TREE);
}

/// <summary>
/// compare between 2 string id
/// </summary>
/// <param name="ID1">ptr to ID1</param>
/// <param name="ID2">ptr to ID2</param>
/// <returns>0 - the same Id, -1 - id1 smaller then id2, 1 id1 bigger then id2</returns>
int compareID(char* ID1, char* ID2)
{
	return strcmp(ID1, ID2);
}

/// <summary>
/// Goes through the whole tree and sends the patients' details to be printed
/// </summary>
/// <param name="root">ptr to root of the tree</param>
void printAllPatient(pInTree* root)
{
	if (root == TREE_END) return;
	printPatient(root->tpatient); //print data
	printAllPatient(root->right); 
	printAllPatient(root->left);
	return;
}

/// <summary>
/// Goes through the entire tree and sends the patients' details for printing (name and ID card only)
/// </summary>
/// <param name="root"></param>
void printAllPatientNameID(pInTree* root)
{
	if (root == TREE_END) return;
	printPatientNameID(root->tpatient); //print data
	printAllPatientNameID(root->right);
	printAllPatientNameID(root->left);
	return;
}

/// <summary>
/// Deletes the patient from the tree according to a given ID
/// </summary>
/// <param name="ptree">ptr to BST ADT</param>
/// <param name="ID">id to delete</param>
void removeFromPTree(pTree* ptree, char* ID)
{
	ptree->root = deleteNode(ptree->root, ID);
}

/// <summary>
/// Finds the patient you want to remove, deletes the patient and arranges the other patients that were under him in the tree
/// </summary>
/// <param name="root">ptr to tree root</param>
/// <param name="ID">id to delete</param>
/// <returns>the new tree root</returns>
pInTree* deleteNode(pInTree* root, char* ID)
{
	if (root == EMPTY_TREE) return NULL; //if the tree is empty

	int compID = compareID(root->tpatient->ID, ID); //find the id in the tree
	if (compID < 0) root->left = deleteNode(root->left, ID); //go left
	else if (compID > 0) root->right = deleteNode(root->right, ID); //go right
	else { //if we found the patient
		if (root->left == NULL) { //if there are no left children
			pInTree* temp = root->right;
			free(root->tpatient);
			free(root);
			return temp;
		}
		else if (root->right == NULL) { //if there are no right children
			pInTree* temp = root->left;
			free(root->tpatient);
			free(root);
			return temp;
		}
		//if there is left and right children:
		 
		free(root->tpatient);
		root->tpatient = findMin(root->right)->tpatient; //Finds the smallest ID in the right subtree and puts the pointer to the patient in the root
		free(findMin(root->right));
		/*
		strcpy(root->tpatient->Name, temp->tpatient->Name);
		strcpy(root->tpatient->ID, temp->tpatient->ID);
		root->tpatient->Allergies = temp->tpatient->Allergies;
		root->tpatient->visits = temp->tpatient->visits;
		root->tpatient->nVisits = temp->tpatient->nVisits;
		*/
		




		//root->right = deleteNode(root->right, temp->tpatient->ID); //delete the old right subtree
	}
	return root;
}

/// <summary>
/// Find the patient with the smallest ID from the right subtree
/// </summary>
/// <param name="root">root to right subtree</param>
/// <returns>the new right subtree root</returns>
pInTree* findMin(pInTree* root)
{
	pInTree* current = root;
	while (current && current->left != NULL)
		current = current->left;
	return current;
}