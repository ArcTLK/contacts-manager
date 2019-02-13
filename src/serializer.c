#include "serializer.h"
#include <string.h>
#include <stdio.h>

void serialize(contact *contacts, char *output, int *numOfContacts) {
    int i;
    for (i = 0; i < *numOfContacts; ++i) {
        strcat(output, "<name>");
        strcat(output, contacts[i].name);
        strcat(output, "</name>");
        strcat(output, "<number>");
        strcat(output, contacts[i].number);
        strcat(output, "</number>");
    }
}

int unserialize(char *input, contact* contacts) {
    int contactsIndex = 0;
    int readElementHead = 0;
    int readElementContent = 0;
    char element[50];
    char elementContent[100];
    int elementIndex = 0;
    int elementContentIndex = 0;
    size_t i;
    for (i = 0; i < strlen(input); ++i) {
        if (input[i] == '<' && !readElementContent) {
            readElementHead = 1;
        }
        else if (input[i] == '/' && readElementHead) {
            readElementHead = 0;
        }
        else if (input[i] == '>' && readElementHead) {
            readElementHead = 0;
            readElementContent = 1;
        }
        else if (input[i] == '<' && readElementContent) {
            readElementContent = 0;
        }
        else if (input[i] == '>' && !readElementHead) {
            if (strcmp(element, "name") == 0) {
                strcpy(contacts[contactsIndex].name, elementContent);
            }
            else if (strcmp(element, "number") == 0) {
                strcpy(contacts[contactsIndex++].number, elementContent);
            }
            //reset strings
            memset(element, 0, sizeof(element));
            memset(elementContent, 0, sizeof(elementContent));
            elementIndex = 0;
            elementContentIndex = 0;
        }
        else if (readElementHead) {
            element[elementIndex++] = input[i];
        }
        else if (readElementContent) {
            elementContent[elementContentIndex++] = input[i];
        }
    }
    //returning number of contacts
    return contactsIndex;
}
