#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//My Libraries:
#include "structure.h"
#include "filesFunctions.h"

#include "dlist.h" //SLL
#include "ptree.h" //BST
#include "pline.h" //LINE
#include "pstack.h" //STACK

//files name:
#define DOCTOR_FILE_NAME "Doctors.txt"
#define LINE_FILE_NAME "Line.txt"
#define PATIENTS_FILE_NAME "Patients.txt"

//error types

typedef enum {
	Memory,
	WrongInputIDLength,
	WrongInputIDLetters,
	WrongInputName,
	WrongDateInput,
	NoDoctorAvailable,
	AlreadyInSystem,
	AlreadyInSystemAndLine,
	AlreadyInLine,
	IDNotFound,
	PatientNotInLine,
	NoVisits,
	VisitNotFound,
	Menu
} Errors;

//Function Prototypes:
//menu functions:

int menu();
void admit_patient(pTree* ptree, docList* list, pLine* line);
void check_for_patient_allergies(pLine* line, pTree* tree);
void display_all_patients(pTree* tree);
void display_all_patient_admissions(pTree* tree);
void display_all_patients_in_line(pLine* line);
void advance_patient_in_line(pLine* line);
void display_list_of_doctors(docList* list);
void display_all_patients_assigned_2_doctor(docList* list, pLine* line);
void discharge_patient(docList* list, pLine* line);
void remove_visit(pTree* tree, pLine* line, docList* list);
void remove_patient(pTree* tree, pLine* line, docList* list);
void close_the_hospital(pTree* tree, pLine* line, docList* list, int* action);


//other functions:

void displayError(enum Errors error);
Doc* assignDoctor2case(docList* list);
char addAllergies(char all, char add);
int checkID(char* id);
void enterID(char* id);
int checkName(char* name);
Doc* checkDoctorAvailable(docList* list);
void insertCurrentTime(Date* date);

int main()
{
	//init data:
	//init : doctor list, patients line, patient tree
	docList* doctorList = initDocList();
	pLine* pline = initLine();
	pTree* ptree = initPTree();
	//get the data from the files:
	loadDoctors(DOCTOR_FILE_NAME, doctorList);
	loadPatients(PATIENTS_FILE_NAME, ptree, doctorList);
	loadLine(LINE_FILE_NAME, pline, ptree);
	printf("wellcome to the system!\n");
	int action = -1;

	while (action != 0)
	{
		action = menu();
		switch (action)
		{
		case 0:
			updateFiles(DOCTOR_FILE_NAME, LINE_FILE_NAME, PATIENTS_FILE_NAME, ptree, doctorList, pline);
			destroyLine(pline);
			destroyPTree(ptree);
			destroyDocList(doctorList);
			printf("********************\n\nThe hospital is closed and all information is kept in files. You exit the system and it is closed\n");
			break;
		case 1:
			printf("********************\n\nadmit patient:\n");
			admit_patient(ptree, doctorList, pline);
			break;
		case 2:
			printf("********************\n\nCheck for patient allergies:\n");
			check_for_patient_allergies(pline, ptree);
			break;
		case 3:
			printf("********************\n\nDisplay all patients:\n");
			display_all_patients(ptree);
			break;
		case 4:
			printf("********************\n\nDisplay all patients admissions:\n");
			display_all_patient_admissions(ptree);
			break;
		case 5:
			printf("********************\n\nDisplay all patients in the line:\n");
			display_all_patients_in_line(pline);
			break;
		case 6:
			printf("********************\n\nAdvance patient in the line:\n");
			advance_patient_in_line(pline);
			break;
		case 7:
			printf("********************\n\ndisplay list of doctors:\n");
			display_list_of_doctors(doctorList);
			break;
		case 8:
			printf("********************\n\ndisplay all patients assigned to doctor:\n");
			display_all_patients_assigned_2_doctor(doctorList, pline);
			break;
		case 9:
			printf("********************\n\ndischarge patient:\n");
			discharge_patient(doctorList, pline);
			break;
		case 10:
			printf("********************\n\nremove visit:\n");
			remove_visit(ptree, pline, doctorList);
			break;
		case 11:
			printf("********************\n\nremove patient:\n");
			remove_patient(ptree, pline, doctorList);
			break;
		case 12:
			printf("********************\n\nclose the hospital:\n");
			close_the_hospital(ptree, pline, doctorList, &action);
			if (action == 0)
			{
				ptree = NULL;
				pline = NULL;
				doctorList = NULL;
				updateFiles(DOCTOR_FILE_NAME, LINE_FILE_NAME, PATIENTS_FILE_NAME, ptree, doctorList, pline);
				printf("********************\n\nThe hospital was closed and all information was deleted. You exit the system and it is closed\n");
			}
			break;
		case -1:
			displayError(Menu);
			break;
		}
	}
	return 0;
}


// Definition of Functions:

/// <summary>
/// print the menu and return the action
/// </summary>
/// <returns>action number</returns>
int menu()
{
	printf("\n********************************\n\nMenu options:\n"
		"1. Admit patient\n"
		"2. Check for patient's allergies\n"
		"3. Display all patients\n"
		"4. Display all patient's admissions\n"
		"5. Display all patients in line\n"
		"6. Advance patient in line\n"
		"7. Display list of doctors\n"
		"8. Display all patients assigned to a doctor\n"
		"9. Discharge patient\n"
		"10. Remove visit\n"
		"11. Remove patient\n"
		"12. Close the hospital\n"
		"0. Exit program\n\n"
		"enter the number you want to do: ");
	int choise;
	scanf("%d", &choise);
	fseek(stdin, 0, SEEK_END);
	if (choise < 13 && choise > -1) return choise;
	else return -1;
}

//menu functions:

/// <summary>
/// inserting new patient to the system.
/// </summary>
/// <param name="ptree"> pointer to ptree</param>
/// <param name="list"> pointer to doctor list</param>
/// <param name="line"> pointer to patient list</param>
void admit_patient(pTree* ptree, docList* list, pLine* line)
{
	if (checkDoctorAvailable(list) == NULL)
	{
		displayError(NoDoctorAvailable);
		return;
	}
	Patient newPatient = { 0 };
	Visit newVisit = { 0 };
	insertCurrentTime(&newVisit.tArrival); // inserting the current date to the new visit.
	newVisit.Duration.min = -1;
	newVisit.Duration.hour = -1;
	newVisit.tDismissed.Day = -1;
	newVisit.tDismissed.Month = -1;
	newVisit.tDismissed.Year = -1;
	newVisit.tDismissed.Min = -1;
	newVisit.tDismissed.Hour = -1;
	enterID(newPatient.ID);

	Patient* patientPtr = find(ptree, newPatient.ID);
	if (patientPtr != NULL)// checking if the patient is already in the hospital's system
	{
		if (patientPtr->visits->head != NULL && patientPtr->visits->head->visit->tDismissed.Year == -1) // if the patient is already in line no new visit is needed, if not, a new visit will be created.
		{
			displayError(AlreadyInSystemAndLine);
			return;
		}
		displayError(AlreadyInSystem);
		printf("\nadding new visit\n");
		newVisit.Doctor = assignDoctor2case(list);
		pushToVisitsStack(patientPtr->visits, newVisit);
		patientPtr->nVisits++;
		addToTail(line, patientPtr);
		return;
	}
	newPatient.visits = initVisitsStack();
	newVisit.Doctor = assignDoctor2case(list);
	newPatient.nVisits++;
	pushToVisitsStack(newPatient.visits, newVisit);// creating a new visit for a new patient and starting to insert his personal information.

	fseek(stdin, 0, SEEK_END);
	printf("please enter the patient's name: ");
	fgets(newPatient.Name, NAME_LENGTH, stdin);
	newPatient.Name[strcspn(newPatient.Name, "\n")] = '\0';
	while (checkName(newPatient.Name) == 0)
	{
		displayError(WrongInputName);
		fseek(stdin, 0, SEEK_END);
		printf("please enter the patient's name: ");
		fgets(newPatient.Name, NAME_LENGTH, stdin);
		newPatient.Name[strcspn(newPatient.Name, "\n")] = '\0';
	}
	char* allergies[] = { "Penicillin","Sulfa","Opioids","Anesthetics","Eggs","Latex","Preservatives",NULL };
	char tempAlergies[NAME_LENGTH];
	while (1) {
		printf("\nPlease enter an allergy (or type 'done' to finish): ");
		scanf("%s", tempAlergies);
		if (strcmp(tempAlergies, "done") == 0) break;

		int i;
		for (i = 0; allergies[i] != NULL; i++) {
			if (strcmp(tempAlergies, allergies[i]) == 0) {
				newPatient.Allergies = addAllergies(newPatient.Allergies, allergies[i]);
				break;
			}
		}
		if (allergies[i] == NULL) printf("Unknown allergy: %s\n", tempAlergies);
	}
	insert2PTree(ptree, newPatient);// adding the new patient information to the tree
	addToTail(line, find(ptree, newPatient.ID)); // adding the new patient to the line
}

/// <summary>
/// checking for the allergies of a specific patient
/// </summary>
/// <param name="line">pointer to line ADT</param>
/// <param name="tree">pointer to tree ADT</param>
void check_for_patient_allergies(pLine* line, pTree* tree)
{
	printPatientInLineNameID(line);
	Patient toPrint = { 0 };
	enterID(toPrint.ID);
	Patient* p = find(tree, toPrint.ID);
	if (p == NULL)
	{
		displayError(IDNotFound);
		return;
	}
	printf("The allergies of this pateint: ");
	printAllergies(p->Allergies);// prints the allergies of the requested patient
}

/// <summary>
/// prints all patients with their personal information
/// </summary>
/// <param name="tree">pointer to tree ADT</param>
void display_all_patients(pTree* tree)
{
	printf("all the patient in the system:\nName			ID	\n---------------------------------------\n");
	printAllPatientNameID(tree->root);
	return;
}

/// <summary>
/// prints all the visits of a specific patient.
/// </summary>
/// <param name="tree">pointer to tree ADT</param>
void display_all_patient_admissions(pTree* tree)
{
	char* tempID[ID_LENGTH];
	enterID(tempID);
	Patient* tempPatient = find(tree, tempID);
	if (tempPatient == NULL)// checks if the requested patient exists in the system, if not , returns to the main menu. if he does exist then printing all of his visit information
	{
		displayError(IDNotFound);
		return;
	}
	printVisitsStack(tempPatient->visits);
}

/// <summary>
/// prints all the patients in line right now.
/// </summary>
/// <param name="line">pointer to line ADT</param>
void display_all_patients_in_line(pLine* line)
{
	if (line->tail == NULL)// if there is no one in line, returning to main menu
	{
		printf("There is no one in line right now, returning to main menu\n\n");
		return;
	}
	pInLine* temp = line->tail;
	printf("Name		ID		Time Arrival		doctor\n");
	printf("last in the line:\n");
	while (temp != NULL)
	{
		printf("%s	%s	%d/%d/%d	%d:%d	 %s\n", temp->lpatient->Name, temp->lpatient->ID,
			temp->lpatient->visits->head->visit->tArrival.Day,
			temp->lpatient->visits->head->visit->tArrival.Month,
			temp->lpatient->visits->head->visit->tArrival.Year,
			temp->lpatient->visits->head->visit->tArrival.Hour,
			temp->lpatient->visits->head->visit->tArrival.Min,
			temp->lpatient->visits->head->visit->Doctor->Name);
		temp = temp->next;
	}
	printf("first in the line:\n");
}

/// <summary>
/// advancing patient in  line
/// </summary>
/// <param name="line">pointer to line ADT</param>
void advance_patient_in_line(pLine* line)
{
	printPatientInLineNameID(line);
	char advancedID[ID_LENGTH];
	enterID(advancedID);
	pInLine* toAdvance = findInLine(line, advancedID);
	if (toAdvance == NULL)
	{
		displayError(PatientNotInLine);
		return;
	}
	if (line->head == line->tail || toAdvance == line->head) return;// advancing the patient in line while fixing the new line order
	if (toAdvance == line->tail)
	{
		line->tail = line->tail->next;
	}
	else
	{
		pInLine* temp = line->tail;
		while (temp->next != toAdvance) temp = temp->next;
		temp->next = temp->next->next;
	}
	line->head->next = toAdvance;
	line->head = toAdvance;
	line->head->next = LINE_END;
	printLine;
}

/// <summary>
/// prints the list of all the doctors and their information
/// </summary>
/// <param name="list">pointer to the doctor's list</param>
void display_list_of_doctors(docList* list)
{
	printDocList(list);
}

/// <summary>
/// prints all the of the requested doctor current patients
/// </summary>
/// <param name="list">pointer to the list of doctors</param>
/// <param name="line">pointer to line ADT</param>
void display_all_patients_assigned_2_doctor(docList* list, pLine* line)
{
	printDocList(list);
	char docName[NAME_LENGTH];
	printf("please enter doctor name: ");
	fgets(docName, NAME_LENGTH, stdin);
	docName[strcspn(docName, "\n")] = '\0';
	while (checkName(docName) == 0)
	{
		displayError(WrongInputName);
		printf("please enter doctor name: ");
		fgets(docName, NAME_LENGTH, stdin);// inserting the doctor name and looks for all the patients in line that are assigned to him
		docName[strcspn(docName, "\n")] = '\0';
	}
	Doc* doctorToPrint = findDoctor(list, docName);
	if (doctorToPrint == NULL) return;
	pInLine* temp = line->tail;
	while (temp != LINE_END)
	{
		if (strcmp(temp->lpatient->visits->head->visit->Doctor->Name, docName) == 0)
		{
			printf("%s		%s		", temp->lpatient->Name, temp->lpatient->ID);
			printDate(temp->lpatient->visits->head->visit->tArrival, 2);
			printf("\n");
		}
		temp = temp->next;
	}
	return;
}

/// <summary>
/// closing a visit of a patient and letting him go of the hospital
/// </summary>
/// <param name="list">pointer to the list of doctors</param>
/// <param name="line">pointer to line ADT</param>
void discharge_patient(docList* list, pLine* line)
{
	char tempID[ID_LENGTH];
	enterID(tempID);
	pInLine* toDischarge = findInLine(line, tempID);
	if (toDischarge == NULL)
	{
		displayError(PatientNotInLine);
		return;
	}
	insertCurrentTime(&(toDischarge->lpatient->visits->head->visit->tDismissed));// inserting the hour of dissmisal to the visit information
	struct tm tm1, tm2;
	tm1.tm_year = toDischarge->lpatient->visits->head->visit->tArrival.Year-1900;
	tm1.tm_mon = toDischarge->lpatient->visits->head->visit->tArrival.Month-1;
	tm1.tm_mday = toDischarge->lpatient->visits->head->visit->tArrival.Day;
	tm1.tm_hour = toDischarge->lpatient->visits->head->visit->tArrival.Hour;
	tm1.tm_min = toDischarge->lpatient->visits->head->visit->tArrival.Min;
	tm1.tm_sec = 0;
	tm1.tm_isdst = 0;

	tm2.tm_year = toDischarge->lpatient->visits->head->visit->tDismissed.Year-1900;
	tm2.tm_mon = toDischarge->lpatient->visits->head->visit->tDismissed.Month-1;
	tm2.tm_mday = toDischarge->lpatient->visits->head->visit->tDismissed.Day;
	tm2.tm_hour = toDischarge->lpatient->visits->head->visit->tDismissed.Hour;
	tm2.tm_min = toDischarge->lpatient->visits->head->visit->tDismissed.Min;
	tm2.tm_sec = 0;
	tm2.tm_isdst = 0;
	
	time_t tm1sec = mktime(&tm1);
	time_t tm2sec = mktime(&tm2);
	int diffTime = (int)difftime(tm2sec, tm1sec); //in sec
	diffTime = diffTime / 60; //in min

	toDischarge->lpatient->visits->head->visit->Duration.hour = diffTime/60;
	toDischarge->lpatient->visits->head->visit->Duration.min = diffTime%60;
	
	printf("enter visit summary: ");// entering a summery of the patient visit
	fseek(stdin, 0, SEEK_END);
	fgets(toDischarge->lpatient->visits->head->visit->vSummary, SUMMARY_LENGTH, stdin);
	toDischarge->lpatient->visits->head->visit->Doctor->nPatients--;
	printVisit(toDischarge->lpatient->visits->head->visit);
	if (line->head == line->tail || toDischarge == line->head)
	{
		removeFromHead(line);
	}
	else if (toDischarge == line->tail)
	{
		line->tail = line->tail->next;
		free(toDischarge);
	}
	else
	{
		pInLine* temp = line->tail;
		while (temp->next != toDischarge) temp = temp->next;
		temp->next = temp->next->next;
		free(toDischarge);
	}
	printPatientInLineNameID(line);
}

/// <summary>
/// remove a specific visit from the system
/// </summary>
/// <param name="tree">pointer to tree ADT</param>
/// <param name="line">pointer to line ADT</param>
/// <param name="list">pointer to the list of docotrs</param>
void remove_visit(pTree* tree, pLine* line, docList* list)
{
	char tempID[ID_LENGTH];
	enterID(tempID);
	Patient* tempPatient = find(tree, tempID);
	if (tempPatient == NULL)
	{
		displayError(IDNotFound);
		return;
	}
	if (tempPatient->nVisits == 0)// if there are no visits for this patient returning to main menu
	{
		displayError(NoVisits);
		return;
	}
	printVisitsStack(tempPatient->visits);
	Date tempDate = {0};
	printf("Please enter the date of the visit you would like to delete:\nin the format - day/month/year: ");
	while (scanf("%d/%d/%d", &tempDate.Day, &tempDate.Month, &tempDate.Year) != 3)// gets the date of the visit the user would like to delete
	{
		displayError(WrongDateInput);
		fseek(stdin, 0, SEEK_END);
		printf("Please enter the date of the visit you would like to delete:\nin the format - day/month/year: ");
	}
	
	Visit_node* tempVisitNode = tempPatient->visits->head;
	if (tempPatient->visits->head->visit->tArrival.Day == tempDate.Day &&
		tempPatient->visits->head->visit->tArrival.Month == tempDate.Month &&
		tempPatient->visits->head->visit->tArrival.Year == tempDate.Year)
	{
		if (tempPatient->visits->head->visit->tDismissed.Year == -1) //if the requested visit was on going, (patient was yet to be discharged)
		{
			tempPatient->visits->head->visit->Doctor->nPatients--;
			if (line->head == line->tail)
			{
				removeFromHead(line);
			}
			else if (tempPatient == line->tail->lpatient)
			{
				line->tail = line->tail->next;
			}
			else
			{
				pInLine* temp = line->tail;
				while (temp->next->lpatient != tempPatient) temp = temp->next;
				temp->next = temp->next->next;
			}
			line->head->next = LINE_END;
		}
		Visit_node* toDelete = tempPatient->visits->head;
		tempPatient->visits->head = tempPatient->visits->head->next;
		tempPatient->nVisits--;
		free(toDelete);
		printVisitsStack(tempPatient->visits);// prints the remainig visits
		return;
	}
	while (tempVisitNode->next != STACK_END)
	{
		if (tempVisitNode->next->visit->tArrival.Day == tempDate.Day &&
			tempVisitNode->next->visit->tArrival.Month == tempDate.Month &&
			tempVisitNode->next->visit->tArrival.Year == tempDate.Year)
		{
			Visit_node* toDelete = tempVisitNode->next;
			tempVisitNode->next = tempVisitNode->next->next;
			tempPatient->nVisits--;
			free(toDelete);
			printf("\nThe visit was succesfully deleted!\n");
			printVisitsStack(tempPatient->visits);
			return;
		}
		tempVisitNode = tempVisitNode->next;
	}
	displayError(VisitNotFound);
	return;
}

/// <summary>
/// deletes a requested patient from the system
/// </summary>
/// <param name="tree"></param>
/// <param name="line"></param>
/// <param name="list"></param>
void remove_patient(pTree* tree, pLine* line, docList* list) //לבדוק את הפונקציה הזאת!!!!! בעיה במחיקת האיבר הראשון בעץ (תז123456789)
{
	char tempID[ID_LENGTH];
	enterID(tempID);
	Patient* tempPatient = find(tree, tempID);
	if (tempPatient == NULL)
	{
		displayError(IDNotFound);
		printf("\nreturning to main menu\n\n");
		return;
	}
	if (tempPatient->visits->head != EMPTY_STACK && tempPatient->visits->head->visit->tDismissed.Year == -1)// if the patient is in line, removing him from the line.
	{
		pInLine* temp = line->tail;
		pInLine* toDelete;
		if (line->head->lpatient == tempPatient)
		{
			removeFromHead(line);
		}
		else if (tempPatient == line->tail->lpatient)
		{
			toDelete = line->tail;
			line->tail = line->tail->next;
			free(toDelete);
		}
		else
		{
			while (temp->next->lpatient != tempPatient) temp = temp->next;
			toDelete = temp->next;
			temp->next = temp->next->next;
			free(toDelete);
		}
		line->head->next = LINE_END;
		tempPatient->visits->head->visit->Doctor->nPatients--;
	}
	removeFromPTree(tree, tempPatient->ID);// removig the patient from the hospital system.
	printf("Patient succesfully removed ! \n");
}

/// <summary>
/// shutting down the hospital for good and deleting all of his information
/// </summary>
/// <param name="tree">pointer to tree ADT</param>
/// <param name="line">pointer to line ADT</param>
/// <param name="list">pointer to the lsit of doctors</param>
/// <param name="action">pointer to the action bottom</param>
void close_the_hospital(pTree* tree, pLine* line, docList* list, int* action)
{
	printf("are you sure you want to close the hospital?\n1 - yes, 0 - no\n");// making sure the user would like to delete all of the hospital infomatin.
	int choice;
	while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1))
	{
		fseek(stdin, 0, SEEK_END);
		printf("are you sure you want to close the hospital?\n1 - yes, 0 - no\n");
	}

	if (choice == 1) // deleting all of the saved information
	{
		destroyLine(line);
		destroyPTree(tree);
		destroyDocList(list);
		*action = 0;// changes the action value in the main menu to 0 so it would close the application immmidiatly after this command
		return;
	}
	else if (choice == 0)// if the user chose not to delete all of the hospital infomation returning to main menu
	{
		printf("Returning to main menu... \n");
		return;
	}
	
}


//other functions:

/// <summary>
/// Receives an error value and prints an appropriate message
/// </summary>
void displayError(enum Errors error)
{
	switch (error)
	{
	case Memory:
		printf("A problem with dynamic memory allocation, the program will shut down and all recent changes will not be saved\n");
		break;
	case WrongInputIDLength:
		printf("Wrong input lengh, please enter 9 digit ID\n");
		break;
	case WrongInputIDLetters:
		printf("Wrong input, please enter ONLY digits from 0-9\n");
		break;
	case WrongInputName:
		printf("Error typing the name, you must type a name that contains only letters\n");
		break;
	case WrongDateInput:
		printf("Error typing the date, you must type a date int the format - day/month/year\n");
		break;
	case NoDoctorAvailable:
		printf("there is no doctor available :(\n");
		break;
	case AlreadyInSystem:
		printf("Patient already in the system\n");
		break;
	case AlreadyInSystemAndLine:
		printf("Patient already in the system, and in line. you sent back to menu\n");
		break;
	case AlreadyInLine:
		printf("Patient already in the system line\n");
		break;
	case IDNotFound:
		printf("this ID not found in the system\n");
		break;
	case PatientNotInLine:
		printf("this patient not found in the line\n");
		break;
	case NoVisits:
		printf("this patient has no visits\n");
		break;
	case VisitNotFound:
		printf("this visit not found in the system\n");
		break;
	case Menu:
		printf("\n*****wrong input, chose number from 0 to 12*****\n");
		break;
	default:
		printf("An unknown error has occurred\n");
		break;
	}
}

/// <summary>
/// adding the allergies to the patients personal information
/// </summary>
/// <param name="all">pointer to the list of allergies</param>
/// <param name="add">pointer to the requested allergie </param>
/// <returns></returns>
char addAllergies(char all, char add)
{
	return all | add;// returning the list of patients allergies
}

/// <summary>
/// checks that the input id was correctly typed
/// </summary>
/// <param name="id">pointer to the requested id to check</param>
/// <returns></returns>
int checkID(char* id)
{
	if (strlen(id) != ID_LENGTH - 1)// checks lengh
	{
		displayError(WrongInputIDLength);
		return 0;
	}
	while (*id)
	{
		if (!isdigit(*id))// checks that the input is only digits
		{
			displayError(WrongInputIDLetters);
			return 0;
		}
		id++;
	}
	return 1;
}

/// <summary>
/// function for entering an id for genenral use in the application
/// </summary>
/// <param name="id">pointer to the request memory slot to type in the id</param>
void enterID(char* id)
{
	printf("Please enter Patient's ID number: ");
	scanf("%s", id);
	while (checkID(id)==0) 
	{
		printf("Please enter Patient's ID number: ");
		scanf("%s", id);
	}
	return;
}

/// <summary>
/// checking that a name input is written correctrly
/// </summary>
/// <param name="name">pointer to the name input</param>
/// <returns></returns>
int checkName(char* name)
{
	int i = 0;
	if (name[0] == '\0') return 0;
	while (name[i] != '\0') {
		if (!isalpha(name[i]) && name[i] != ' ') return 0;
		i++;
	}
	for (i = 0; name[i] != '\0'; i++) {
		name[i] = tolower(name[i]);
	}
	name[0] = toupper(name[0]);
	for (i = 1; name[i] != '\0'; i++) {
		if (name[i - 1] == ' ') name[i] = toupper(name[i]);
	}
	return 1;
}

/// <summary>
/// check if there is a doctor with less then 4 patients 
/// </summary>
/// <param name="list">pointer to the list of dotors</param>
/// <returns></returns>
Doc* checkDoctorAvailable(docList* list)
{
	Doc* tempDoc = NULL;
	Doc_node* tempNode = list->head->next;
	if (list->head->doctor->nPatients < tempNode->doctor->nPatients) tempDoc = list->head->doctor;// looks for the doctor with the least amount of patients
	else tempDoc = tempNode->doctor;
	tempNode = tempNode->next;
	while (tempNode->next != NULL)
	{
		if (tempDoc->nPatients > tempNode->doctor->nPatients) tempDoc = tempNode->doctor;
		tempNode = tempNode->next;
	}
	if (tempDoc->nPatients >= 4)// if there is no doctor with less then 4 patients, returning to main menu
	{
		displayError(NoDoctorAvailable);
		printf("\nreturning to main menu\n");
		return NULL;

	}
	return tempDoc;
}

/// <summary>
/// assighning the most free docotor to patient's case
/// </summary>
/// <param name="list">pointer to the list of doctors</param>
/// <returns></returns>
Doc* assignDoctor2case(docList* list)
{
	Doc* tempDoc = checkDoctorAvailable(list);// checks if there is a doctor available
	if (tempDoc == NULL)
	{
		return NULL;
	}
	tempDoc->nPatients++;// adds the number of patients assigned to the doctor by one and sends back a pointer to him
	return tempDoc;
}

/// <summary>
/// inserting the current time and date
/// </summary>
/// <param name="date">a date structure for general use</param>
void insertCurrentTime(Date* date)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	date->Day = tm.tm_mday;
	date->Month = tm.tm_mon + 1;
	date->Year = tm.tm_year + 1900;
	date->Hour = tm.tm_hour;
	date->Min = tm.tm_min;
}
