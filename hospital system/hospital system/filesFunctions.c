#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "filesFunctions.h"

/// <summary>
/// Skips the header and for each patient takes out name, ID, and allergies. and then controls information about all his visits
/// </summary>
/// <param name="fileName"></param>
/// <param name="tree">ptr to BST ADT</param>
/// <param name="list">ptr to list ADT</param>
void loadPatients(char* fileName, pTree* tree, docList* list)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	fseek(file, 47, SEEK_SET); //skip the header
	char tempData[100];

	while (fgets(tempData, 100, file) != NULL && tempData[0] != '\n') //for each patient in the file
	{
		//patient data setup:
		Patient p; 
		p.visits = initVisitsStack();
		p.nVisits = 0;
		int c = 0;
		char name[30] = { 0 };
		char ID[ID_LENGTH] = { 0 };
		char allergies = none;
		int inCounter = 0;
		while (tempData[c] != '.')
		{
			c++;
		}
		c++;
		while (tempData[c] != ';') //get the name
		{
			name[inCounter] = tempData[c];
			c++;
			inCounter++;
		}
		strcpy(p.Name, name); //insert to the patient struct
		c += 1; //skip the ";"
		inCounter = 0;
		while (tempData[c] != ';') //get the ID
		{
			ID[inCounter] = tempData[c];
			c++;
			inCounter++;
		}
		strcpy(p.ID, ID); //insert to the patient struct
		c += 1; //skip the ";"
		while (tempData[c] != '\0') //Reads the name of the allergy, identifies it and adds it to a char that contains all the allergies
		{
			inCounter = 0;
			char allergy[NAME_LENGTH] = { 0 };
			while (tempData[c] != ',' && tempData[c] != '\n')
			{
				allergy[inCounter] = tempData[c];
				c += 1;
				inCounter += 1;
			}
			if (strcmp(allergy, "Penicillin") == 0) allergies = addAllergies(allergies, Penicillin);
			if (strcmp(allergy, "Sulfa") == 0) allergies = addAllergies(allergies, Sulfa);
			if (strcmp(allergy, "Opioids") == 0) allergies = addAllergies(allergies, Opioids);
			if (strcmp(allergy, "Anesthetics") == 0) allergies = addAllergies(allergies, Anesthetics);
			if (strcmp(allergy, "Eggs") == 0) allergies = addAllergies(allergies, Eggs);
			if (strcmp(allergy, "Latex") == 0) allergies = addAllergies(allergies, Latex);
			if (strcmp(allergy, "Preservatives") == 0) allergies = addAllergies(allergies, Preservatives);

			c += 1;
		}
		p.Allergies = allergies; //insert to the patient struct
		fseek(file, 10, SEEK_CUR);
		fgets(tempData, 100, file);

		while (tempData[0] != 61)//61 - "="     for all the visits antil find the divder "============..."
		{
			Visit visit = { 0 };
			sscanf(tempData, "%d/%d/%d %d:%d", &visit.tArrival.Day, &visit.tArrival.Month, &visit.tArrival.Year, &visit.tArrival.Hour, &visit.tArrival.Min); //tArrival

			fseek(file, 10, SEEK_CUR); 
			fgets(tempData, 100, file); //read line
			sscanf(tempData, "%d/%d/%d %d:%d", &visit.tDismissed.Day, &visit.tDismissed.Month, &visit.tDismissed.Year, &visit.tDismissed.Hour, &visit.tDismissed.Min); //tDismissed

			fseek(file, 9, SEEK_CUR); 
			fgets(tempData, 100, file); //read line
			sscanf(tempData, "%f:%f", &visit.Duration.hour, &visit.Duration.min); //Duration

			if (visit.tDismissed.Year == 0) //if still in the line
			{
				visit.tDismissed.Day = -1;
				visit.tDismissed.Month = -1;
				visit.tDismissed.Year = -1;
				visit.tDismissed.Min = -1;
				visit.tDismissed.Hour = -1;
				visit.Duration.hour = -1;
				visit.Duration.min = -1;
			}

			fseek(file, 7, SEEK_CUR);
			fgets(tempData, 100, file); //read line
			for (int i = 0; i < strlen(tempData - 1); i++) //read the full name of the doctor
			{
				if (tempData[i] == '\n') {
					tempData[i] = '\0';
					break;
				}
			}
			visit.Doctor = findDoctor(list, tempData); //insert ptr to the doctor structure

			fseek(file, 8, SEEK_CUR); //read line
			fgets(tempData, 100, file);
			visit.vSummary = calloc(100, sizeof(char)); //Allocates memory for the visit summary
			if (visit.vSummary == NULL) {
				printf("A problem with dynamic memory allocation, the program exits and all recent changes will not be saved");
				exit(1);
			}
			strcpy(visit.vSummary, tempData); //insert the summary

			fseek(file, 10, SEEK_CUR);
			fgets(tempData, 100, file); //read line (the divider)
			pushToVisitsStack(p.visits, visit); //add the visit to the stack
			p.nVisits++;
		}
		insert2PTree(tree, p); //add the patient to the tree
	}
	fclose(file);
}

/// <summary>
/// Collects each line and extracts from it the name of the doctor, his license number and the number of patients who are now in line
/// </summary>
/// <param name="fileName"></param>
/// <param name="list">ptr to list ADT</param>
void loadDoctors(char* fileName, docList* list)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	//get down to the data (98chars), skip the header
	fseek(file, 98, SEEK_SET);
	char tempData[100];

	while (fgets(tempData, 100, file) != NULL) 	//while not the end of file get all the line
	{
		int c = 0;
		char name[30] = { 0 };
		char licenseNum[LICENSE_NUMBER] = { 0 };
		int pNum = 0;

		while (tempData[c] != ';') //get all the name antil ";"
		{
			name[c] = tempData[c];
			c++;
		}
		c += 2; //skip the " " space
		int inCounter = 0;

		while (tempData[c] != ';') //get all the licenseNUM antil the ";"
		{
			licenseNum[inCounter] = tempData[c];
			c++;
			inCounter++;
		}
		c += 2; //skip the " " space

		while (tempData[c] != '\n')
		{
			pNum = pNum * 10 + ((int)tempData[c] - 48);
			c++;
		}

		Doc newDoc = { name, "", pNum };
		strcpy(newDoc.nLicense, licenseNum);
		addToHeadDocList(list, newDoc); //add all the doc data to doctor list
	}
	fclose(file);
}

/// <summary>
/// Picks up each row, takes the ID cards out of it, finds the patient in the tree and adds a pointer to him in the queue
/// </summary>
/// <param name="fileName"></param>
/// <param name="line">ptr to line ADT</param>
/// <param name="tree">ptr to BST ADT</param>
void loadLine(char* fileName, pLine* line, pTree* tree)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	fseek(file, 46, SEEK_SET);//skip the header "Patients' IDs in line\n=====================\n"
	char tempID[ID_LENGTH + 8]; //leave place for number and the'.' and \n

	while (fgets(tempID, ID_LENGTH + 8, file) != NULL) 	//while not the end get the id to the line
	{
		char* ptr = strchr(tempID, '.'); //find the '.'
		sscanf(ptr+1, "%s\n", &tempID); //start read after the '.' and before the \n -> insert to tempID
		addToTail(line, find(tree, &tempID)); //add to the end of the line
	}
	fclose(file);
}

/// <summary>
/// Sends each list or tree to the appropriate file for updating
/// </summary>
/// <param name="doctorFileName"></param>
/// <param name="lineFileName"></param>
/// <param name="patientFileName"></param>
/// <param name="tree"> ptr to tree ADT</param>
/// <param name="list"> ptr to list ADT</param>
/// <param name="line"> ptr to line ADT</param>
void updateFiles(char* doctorFileName, char* lineFileName, char* patientFileName, pTree* tree, docList* list, pLine* line)
{
	updateDoctors(doctorFileName, list);
	updateLine(lineFileName, line);
	updatePatients(patientFileName, tree);
}

/// <summary>
/// Prints a header and sends to print all the patients in the tree
/// </summary>
/// <param name="fileName"></param>
/// <param name="tree">ptr to tree ADT</param>
void updatePatients(char* fileName, pTree* tree)
{
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	fprintf(file, "Name; ID; Allergies\n"); //print header
	fprintf(file, "========================\n");
	if (tree == NULL) return;
	int c = 1;
	printPTreeToFile(tree->root, &c, file); //print all the patient in the tree
	fclose(file);
}

/// <summary>
/// Goes through the entire tree and prints the patients in the appropriate format
/// </summary>
/// <param name="root">ptr to tree root</param>
/// <param name="c">ptr to counter for the print</param>
/// <param name="file">ptr to the file</param>
void printPTreeToFile(pInTree* root, int* c, FILE* file)
{
	if (root == TREE_END) return; //if the end of the tree

	fprintf(file, "%d.%s;%s;",*c, root->tpatient->Name, root->tpatient->ID);
	(*c)++;
	printAllergiesToFile(root->tpatient->Allergies, file);
	fprintf(file, "\n");
	printVstackToFile(root->tpatient->visits->head, file); //print all the visits
	fprintf(file, "========================\n");
	printPTreeToFile(root->right, c, file); //go right
	printPTreeToFile(root->left, c, file); //go left
	return;
}

/// <summary>
/// Receives char coded to BITWISE and prints the allergies that patient has to the aright file
/// </summary>
/// <param name="allergies">char coded to BITWISE</param>
/// <param name="file">ptr to file</param>
void printAllergiesToFile(char allergies, FILE* file)
{
	const char* allergy_names[] = { //all the allergies array
		"Penicillin","Sulfa","Opioids","Anesthetics","Eggs","Latex","Preservatives"
	};
	if (allergies == none) {
		fprintf(file,"none");
		return;
	}
	int c = 0;
	for (int i = 0; i < sizeof(allergy_names) / sizeof(allergy_names[0]); i++) { //print all the allergies and add "," between them
		if (allergies & (1 << i)) {
			if (c++ > 0) {
				fprintf(file,", ");
			}
			fprintf(file, "%s", allergy_names[i]);
		}
	}
}

/// <summary>
/// Receives a stack of visits and prints them from old to new recursively
/// </summary>
/// <param name="visitNode">ptr to visit node</param>
/// <param name="file">ptr to file</param>
void printVstackToFile(Visit_node* visitNode, FILE* file)
{
	if (visitNode == EMPTY_STACK) return; //if the end of the stack
	printVstackToFile(visitNode->next, file); //go to the lowest visit
	fprintf(file, "\nArrival:%02d/%02d/%d %02d:%02d\n", visitNode->visit->tArrival.Day, visitNode->visit->tArrival.Month, visitNode->visit->tArrival.Year, visitNode->visit->tArrival.Hour, visitNode->visit->tArrival.Min);
	if (visitNode->visit->Duration.hour == -1) //if still in the line (donot want to print -1)
	{
		fprintf(file, "Dismissed:\n");
		fprintf(file, "Duration:\n");
		fprintf(file, "Doctor:%s\n", visitNode->visit->Doctor->Name);
		fprintf(file, "Summary:\n");
	}
	else //if the visit close print all the data
	{
		fprintf(file, "Dismissed:%02d/%02d/%d %02d:%02d\n", visitNode->visit->tDismissed.Day, visitNode->visit->tDismissed.Month, visitNode->visit->tDismissed.Year, visitNode->visit->tDismissed.Hour, visitNode->visit->tDismissed.Min);
		fprintf(file, "Duration:%.0f:%.0f\n", visitNode->visit->Duration.hour, visitNode->visit->Duration.min);
		fprintf(file, "Doctor:%s\n", visitNode->visit->Doctor->Name);
		fprintf(file, "Summary:%s", visitNode->visit->vSummary);
	}
}

/// <summary>
/// Goes through all the doctors in the list and prints their data
/// </summary>
/// <param name="fileName">file name</param>
/// <param name="list">ptr to list ADT</param>
void updateDoctors(char* fileName, docList* list)
{
	FILE* file = fopen(fileName, "w"); //open the file
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	fprintf(file, "Full Name; License Number; Number of Patients\n"); //print header
	fprintf(file, "=================================================\n");
	if (list == NULL) return;
	Doc_node* temp = list->head;

	while (temp != LIST_END) //for all the doctor in the list
	{
		fprintf(file, "%s; %s; %d\n", temp->doctor->Name, temp->doctor->nLicense, temp->doctor->nPatients);
		temp = temp->next;
	}
	fclose(file); //close the file
}

/// <summary>
/// Goes through all the patient in the line and prints their Id and order in the line
/// </summary>
/// <param name="fileName">file name</param>
/// <param name="line">ptr to line ADT</param>
void updateLine(char* fileName, pLine* line)
{
	FILE* file = fopen(fileName, "w"); //open the file
	if (file == NULL) {
		printf("Error opening file!, exit the program and all the data will not be saved\n");
		exit(1);
	}
	fprintf(file, "Patients' IDs in line\n"); //print header
	fprintf(file, "=====================\n");
	if (line == EMPTY_LINE) return; //if there is no line
	if (line->head == EMPTY_LINE) return; //if there is no patient in the line
	Patient* rememberHead = line->head->lpatient;
	pInLine* temp = line->head;
	int c = 1;

	while (temp->lpatient != rememberHead || c == 1) //for each patient print order in the line and ID
	{
		fprintf(file, "%d.%s\n", c, temp->lpatient->ID);
		c++;
		addToTail(line, removeFromHead(line));
		temp = line->head;
	}
	fclose(file); //close the file
}