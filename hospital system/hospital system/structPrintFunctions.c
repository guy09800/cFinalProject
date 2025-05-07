#define _CRT_SECURE_NO_WARNINGS
#include "dlist.h"
#include <stdlib.h>
#include <string.h>

/// <summary>
/// Prints the doctor's name, license number and number of patients.
/// </summary>
/// <param name="doctor">ptr to doctor struct</param>
void printDoc(const Doc* doctor)
{
	printf("name: %s	num license: %s	num patients: %d\n", doctor->Name, doctor->nLicense, doctor->nPatients);
}

/// <summary>
/// Prints the patient's name, ID, what allergies he has and all previous visits
/// </summary>
/// <param name="patient">ptr to paitent struct</param>
void printPatient(const Patient* patient)
{
	printf("%s;%s;", patient->Name, patient->ID);
	printAllergies(patient->Allergies);
	printf("\n\n");
	printVisitsStack(patient->visits); //print all the visits
}

/// <summary>
/// Gets a char and prints all the allergies the patient has.
/// </summary>
/// <param name="allergies">bitwise char</param>
void printAllergies(char allergies)
{
	const char* allergy_names[] = {
		"Penicillin","Sulfa","Opioids","Anesthetics","Eggs","Latex","Preservatives"
	};
	if (allergies == none) {
		printf("none");
		return;
	}
	int c = 0;
	for (int i = 0; i < sizeof(allergy_names) / sizeof(allergy_names[0]); i++) { //check bitwise and print the right allergy
		if (allergies & (1 << i)) {
			if (c++ > 0) { 
				printf(", ");
			}
			printf("%s", allergy_names[i]);
		}
	}
}

/// <summary>
/// Prints the patient's name and ID.
/// </summary>
/// <param name="patient">ptr to patient struct</param>
void printPatientNameID(const Patient* patient)
{
	printf("%s		%s\n", patient->Name, patient->ID);
}

/// <summary>
/// Gets a pointer to a structure of type visit and prints all the details in it.
/// </summary>
/// <param name="visit">ptr to visit struct</param>
void printVisit(const Visit* visit)
{
	printf("Arrival: ");
	printDate(visit->tArrival, 0);
	if (visit->tDismissed.Year != -1) //If the visit is closed and there is an exit time
	{
		printf("\nDismissed: ");
		printDate(visit->tDismissed, 0);
		printf("\nDuration: %0.f:%0.f", visit->Duration.hour, visit->Duration.min);
	}
	else //If the visit is still happening (the patient is still in line)
	{
		printf("\nDismissed: ");
		printf("\nDuration: ");
	}
	printf("\nDoctor:%s", visit->Doctor->Name);
	printf("\nSummary:%s\n", visit->vSummary);
}

/// <summary>
/// Gets a structure of type date, and which rank to print and prints the date details.
/// 0 - day/mon/year
/// 1 - hour:min
/// 2 - day/mon/year hour:min
/// </summary>
/// <param name="date">date struct</param>
/// <param name="level">date format we want to print</param>
void printDate(Date date, int level)
{
	if (level == 0)
	{
		printf("%d/%d/%d", date.Day, date.Month, date.Year);
	}
	else if (level == 1)
	{
		printf("%d:%d", date.Hour, date.Min);
	}
	else if (level == 2)
	{
		printf("%d/%d/%d	%d:%d", date.Day, date.Month, date.Year, date.Hour, date.Min);
	}
}

