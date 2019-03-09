#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "manager.h"
#include "serializer.h"

unsigned int readFromFile(contact **contacts) {
    char temp[BUFFER_SIZE] = "";
    char *XML;
    FILE *file;
    unsigned int numOfContacts;
    unsigned int i;
    
    //space allocation
    XML = (char*)calloc(sizeof temp, sizeof(char));
    *contacts = (contact*)calloc((unsigned int)ceil((sizeof temp - XML_TAG_SUM) / sizeof(contact)), sizeof(contact));

    if (XML == NULL || *contacts == NULL) {
        printf("Error: Memory not allocated!");
        exit(EXIT_FAILURE);
    }
    XML[0] = '\0'; //set null terminating character
    file = fopen(DIR, "r");
    //transfer file content to variable
    i = 1;
    while(fgets(temp, sizeof temp, file)) {
        strcat(XML, temp);
        //increase size
        XML = (char*)realloc(XML, sizeof temp * ++i);
        *contacts = (contact*)realloc(*contacts, (unsigned int)ceil((sizeof temp - XML_TAG_SUM) / sizeof(contact)) * i);
    }
    //convert XML to array of contacts
    numOfContacts = unserialize(XML, *contacts);
    //free
    free(XML);
    fclose(file);
    return numOfContacts;
}

void createContact(char *name, char *number) {
    contact newContact[1];
    FILE *file;
    char *XML = (char *)calloc(strlen(name) + strlen(number) + XML_TAG_SUM, sizeof(char));
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
}

void searchContact(char *name, char **output) {
    contact *contacts;
    unsigned int numOfContacts;
    unsigned int contactsFound = 0;
    unsigned int i;
    //allocate space
    *output = (char*)calloc(NUMBER_SIZE, sizeof(char));
    if (*output == NULL) {
        printf("Error: Memory not allocated!");
        exit(EXIT_FAILURE);
    }

    strcat(*output, "Numbers:\n"); //heading
    //get contacts
    numOfContacts = readFromFile(&contacts);
    //loop through names
    for (i = 0; i < numOfContacts; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            *output = (char*)realloc(*output, NUMBER_SIZE * (++contactsFound + 1));
            strcat(*output, contacts[i].number);
            strcat(*output, "\n");
        }
    }
    //clean
    free(contacts);
    //check if no contacts were found
    if (strcmp(*output, "Numbers:\n") == 0) {
        strcat(*output, "No contacts found!\n");
    }
}
//returns 1 for success, 0 for error
int editContact(unsigned int *index, int *property, char *change) {
    contact *contacts;
    unsigned int numOfContacts;
    FILE *file;
    char *XML;
    numOfContacts = readFromFile(&contacts);
    if (*index >= numOfContacts) {
        //illegal index!
        return 0;
    }
    //allocate space
    XML = (char *)calloc((sizeof(contact) + XML_TAG_SUM) * numOfContacts, sizeof(char));
    if (*property == 1) {
        //clean old
        memset(contacts[*index].name, 0, sizeof contacts[*index].name);
        //copy
        strcpy(contacts[*index].name, change);
    }
    else if (*property == 2) {
        //clean old
        memset(contacts[*index].number, 0, sizeof contacts[*index].number);
        //copy
        strcpy(contacts[*index].number, change);
    }
    else return 0; //invalid property
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
    for(i = *index - 1; i < numOfContacts - 1; i++) {
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