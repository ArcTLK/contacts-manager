#include <stdio.h>
#include <string.h>
#include "manager.h"
#include "serializer.h"

static const char DIR[] = "contacts.txt";

void createContact(char *name, char *number) {
    contact newContact[1];
    FILE *file;
    char XML[200];
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

void searchContact() {
    char contactsXML[1000];
    char test[1000] = "";
    FILE *file;
    contact contacts[100];
    int numOfContacts;
    file = fopen(DIR, "r");
    fgets(contactsXML, 1000, file);
    fclose(file);
    puts(contactsXML);
    numOfContacts = unserialize(contactsXML, contacts);
    //test
    serialize(contacts, test, &numOfContacts); //good this works now
    puts(test);
}
