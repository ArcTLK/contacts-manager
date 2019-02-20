#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manager.h"
#include "serializer.h"

static const char DIR[] = "D:\\contacts-manager\\contacts.txt";

void createContact(char *name, char *number) {
    contact newContact[1];
    FILE *file;
    char XML[strlen(name) + strlen(number)];
    //clean
    memset(XML, 0, sizeof XML);
    //map strings to contact members
    strcpy(newContact[0].name, name);
    strcpy(newContact[0].number, number);
    //serialize
    serialize(newContact, XML, 1);
    //append to file
    file = fopen(DIR, "a");
    fputs(XML, file);
    fclose(file);
}

void searchContact(char *name, char *output) {
    char temp[100] = "";
    char *XML;
    FILE *file;
    contact *contacts;
    int numOfContacts;
    int i;
    //allocate space
    XML = (char*)calloc(sizeof temp, sizeof(char));
    contacts = (contact*)calloc(sizeof temp / 10, sizeof(contact));

    if (XML == NULL || contacts == NULL) {
        printf("Error: Memory not allocated!");
        exit(EXIT_FAILURE);
    }

    XML[0] = '\0'; //set null terminating character
    strcat(output, "Numbers:\n"); //heading
    file = fopen(DIR, "r");
    //transfer file content to variable
    i = 1;
    while(fgets(temp, sizeof temp, file)) {
        strcat(XML, temp);
        //increase size
        XML = (char*)realloc(XML, sizeof temp * ++i);
        contacts = (contact*)realloc(contacts, sizeof temp * i / 10);
    }
    //convert XML to array of contacts
    numOfContacts = unserialize(XML, contacts);
    //loop through names
    for (i = 0; i < numOfContacts; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            strcat(output, contacts[i].number);
            strcat(output, "\n");
        }
    }
    //clean
    fclose(file);
    free(XML);
    free(contacts);
    //check if no contacts were found
    if (strcmp(output, "Numbers:\n") == 0) {
        strcat(output, "No contacts found!\n");
    }
}
