#include <stdio.h>
#include "manager.h"
#include "serializer.h"

static const char DIR[] = "/storage/emulated/0/CppDroid/contacts.xml";

void createContact() {
    contact newContact[1];
    FILE *file;
    puts("Enter name (max length 50):\t");
    fgets(newContact[0].name, 51, stdin);
    puts("Enter number:\t");
    fgets(newContact[0].number, 11, stdin);
    file = fopen(DIR, "a");
    fputs(serialize(newContact), file);
    fclose(file);
}