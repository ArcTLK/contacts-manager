#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

void createContact(char*, char*);
void searchContact(char*, char*);

typedef struct {
    char name[50];
    char number[10];
} contact;

#endif
