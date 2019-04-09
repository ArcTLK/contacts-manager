#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "manager.h"
#include "serializer.h"

int validateNumber(char *number) {
    int numLen = strlen(number);
    int i;
    for (i = 0; i < numLen; i++) {
        //not a number!
        if (!isdigit(number[i])) return ERR_NAN;
    }
    //check if number is unique
    unsigned int contactsFound;
    contact *foundContacts;
    searchContact(number, 1, &foundContacts, &contactsFound);
    if (contactsFound > 0) return ERR_DUPLICATE_CONTACT;
    return 1; //no error
}

unsigned int readFromFile(contact **contacts) {
    char temp[BUFFER_SIZE] = "";
    char *XML;
    FILE *file;
    unsigned int numOfContacts;
    unsigned int i;

    //space allocation
    XML = (char*)calloc(sizeof temp, sizeof(char));
    *contacts = (contact*)calloc(sizeof temp / sizeof(contact), sizeof(contact));

    if (XML == NULL || *contacts == NULL) {
        printf("Error: Memory not allocated!");
        exit(EXIT_FAILURE);
    }
    XML[0] = '\0'; //set null terminating character
    file = fopen(DIR, "r");
    //transfer file content to variable
    i = 2;
    while(fgets(temp, sizeof temp, file)) {
        strcat(XML, temp);
        //increase size
        i += 2;
        XML = (char*)realloc(XML, sizeof temp * i);
        *contacts = (contact*)realloc(*contacts, sizeof temp * i);
    }
    //convert XML to array of contacts
    numOfContacts = unserialize(XML, *contacts);
    //free
    free(XML);
    fclose(file);
    return numOfContacts;
}

int createContact(char *name, char *number) {
    contact newContact[1];
    FILE *file;
    char *XML = (char *)calloc(strlen(name) + strlen(number) + XML_TAG_SUM, sizeof(char));
    int errorId = validateNumber(number);
    if (errorId != 1) return errorId;
    //map strings to contact members
    strcpy(newContact[0].name, name);
    strcpy(newContact[0].number, number);
    //serialize
    serialize(newContact, XML, 1);
    //append to file
    file = fopen(DIR, "a");
    fputs(XML, file);
    //clean
    free(XML);
    fclose(file);
    return 1;
}
int searchContact(char *search, int searchParam, contact **foundContacts, unsigned int *contactsFound) {
    contact *contacts;
    unsigned int numOfContacts;
    unsigned int i;
    *contactsFound = 0;
    //allocate space
    *foundContacts = (contact *)calloc(1, sizeof(contact));
    //get contacts
    numOfContacts = readFromFile(&contacts);
    if (searchParam == 0) {
        //loop through names
        for (i = 0; i < numOfContacts; i++) {
            if (strcmp(contacts[i].name, search) == 0) {
                //copy
                strcpy((*foundContacts + *contactsFound)->name, contacts[i].name);
                strcpy((*foundContacts + *contactsFound)->number, contacts[i].number);
                (*contactsFound)++;
                *foundContacts = (contact *)realloc(*foundContacts, (*contactsFound + 1) * sizeof(contact)); //increase size
            }
        }
    }
    else if (searchParam == 1) {
        //loop through numbers
        for (i = 0; i < numOfContacts; i++) {
            if (strcmp(contacts[i].number, search) == 0) {
                //copy
                strcpy((*foundContacts + *contactsFound)->name, contacts[i].name);
                strcpy((*foundContacts + *contactsFound)->number, contacts[i].number);
                (*contactsFound)++;
                *foundContacts = (contact *)realloc(*foundContacts, (*contactsFound + 1) * sizeof(contact)); //increase size
            }
        }
    }
    else {
        //filter
        for (i = 0; i < numOfContacts; i++) {
            if (strstr(contacts[i].name, search) != NULL || strstr(contacts[i].number, search) != NULL) {
                //copy
                strcpy((*foundContacts + *contactsFound)->name, contacts[i].name);
                strcpy((*foundContacts + *contactsFound)->number, contacts[i].number);
                (*contactsFound)++;
                *foundContacts = (contact *)realloc(*foundContacts, (*contactsFound + 1) * sizeof(contact)); //increase size
            }
        }
    }

    //clean
    free(contacts);
    //check if no contacts found
    if (*contactsFound == 0) return 0;
    else return 1;
}
//returns 1 for success, 0 for error
int editContact(unsigned int *index, int property, char *change) {
    contact *contacts;
    unsigned int numOfContacts;
    FILE *file;
    char *XML;
    numOfContacts = readFromFile(&contacts);
    if (*index >= numOfContacts) {
        //illegal index!
        return ERR_INVALID_INDEX;
    }
    //allocate space
    XML = (char *)calloc((sizeof(contact) + XML_TAG_SUM) * numOfContacts, sizeof(char));
    if (property == 1) {
        //clean old
        memset(contacts[*index].name, 0, sizeof contacts[*index].name);
        //copy
        strcpy(contacts[*index].name, change);
    }
    else if (property == 2) {
        int errorId = validateNumber(change);
        if (errorId != 1) return errorId;
        //clean old
        memset(contacts[*index].number, 0, sizeof contacts[*index].number);
        //copy
        strcpy(contacts[*index].number, change);
    }
    else return ERR_INVALID_PROPERTY; //invalid property
    //write
    file = fopen(DIR, "w");
    serialize(contacts, XML, (int)numOfContacts);
    fputs(XML, file);
    //clean
    fclose(file);
    free(XML);
    free(contacts);
    return 1;
}

//returns 1 for success, 0 for error
int deleteContact(unsigned int *index) {
    contact *contacts;
    unsigned int numOfContacts;
    unsigned int i;
    FILE *file;
    char *XML;
    numOfContacts = readFromFile(&contacts);
    if (*index >= numOfContacts) {
        //illegal index!
        return 0;
    }
    //allocate space
    XML = (char *)calloc((sizeof(contact) + XML_TAG_SUM) * numOfContacts, sizeof(char));
    //delete
    for(i = *index; i < numOfContacts - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    //write
    file = fopen(DIR, "w");
    serialize(contacts, XML, (int)numOfContacts - 1);
    fputs(XML, file);
    //clean
    fclose(file);
    free(XML);
    free(contacts);
    return 1;
}
