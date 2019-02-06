#include "serializer.h"
#include <string.h>

char* serialize(contact *contacts) {
    static char output[1000];
    int i;
    int size = 1 + sizeof(contacts) / sizeof(contact);
    for (i = 0; i < size; ++i) {
        strcat(output, "<name>");
        strcat(output, contacts[i].name);
        strcat(output, "</name>");
        strcat(output, "<number>");
        strcat(output, contacts[i].number);
        strcat(output, "</number>");
    }
    return output;
}

contact* unserialize(char *input) {
    static contact contacts[100];
    int contactsIndex = 0;
    int readElementHead = 0;
    int readElementContent = 0;
    char element[50];
    char elementContent[100];
    int elementIndex = 0;
    int elementContentIndex = 0;
    size_t i;
    int j;
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
                for (j = 0; j <= elementContentIndex; ++j) {
                    contacts[contactsIndex].name[j] = elementContent[j];
                }
            }
            else if (strcmp(element, "number") == 0) {
                for (j = 0; j <= elementContentIndex; ++j) {
                    contacts[contactsIndex].number[j] = elementContent[j];
                }
                contactsIndex++;
            }
            //reset strings
            element[0] = '\0';
            elementContent[0] = '\0';
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
    return contacts;
}