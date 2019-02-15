#include <stdio.h>
#include <string.h>
#include "manager.h"
#include "serializer.h"

static const char DIR[] = "D:\\contacts-manager\\contacts.txt";

void createContact(char *name, char *number) {
    contact newContact[1];
    FILE *file;
    char XML[200] = "";
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
    char contactsXML[1000];
    FILE *file;
    contact contacts[100] = {};
    int numOfContacts;
    int i;
    file = fopen(DIR, "r");
    fgets(contactsXML, 1000, file);
    fclose(file);
    numOfContacts = unserialize(contactsXML, contacts);
    strcat(output, "Numbers:\n");
    //loop through names
    for (i = 0; i < numOfContacts; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            strcat(output, contacts[i].number);
            strcat(output, "\n");
        }
    }
    if (strcmp(output, "Numbers:\n") == 0) {
        strcat(output, "No contacts found!\n");
    }
}
