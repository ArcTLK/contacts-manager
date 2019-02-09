#include <stdio.h>
#include <string.h>
#include "manager.h"
#include "serializer.h"

static const char DIR[] = "/storage/emulated/0/CppDroid/contacts.xml";

void createContact() {
    contact newContact[1];
    FILE *file;
    puts("Enter name (max length 50):\t");
    fgets(newContact[0].name, 51, stdin);
    //remove newline character
    newContact[0].name[strlen(newContact[0].name) - 1] = '\0';
    puts("Enter number:\t");
    fgets(newContact[0].number, 15, stdin);
    newContact[0].number[strlen(newContact[0].number) - 1] = '\0';
    file = fopen(DIR, "a");
    fputs(serialize(newContact), file);
    fclose(file);
}

void searchContact() {
    char contactsXML[1000] = "";
    FILE *file;
    contact *contacts;
    file = fopen(DIR, "r");
    fgets(contactsXML, 1000, file);
    fclose(file);
    contacts = unserialize(contactsXML);
    puts(serialize(contacts));
}