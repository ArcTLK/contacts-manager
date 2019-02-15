#ifndef _manager_h
#define _manager_h

void createContact(char*, char*);
void searchContact(char*, char*);

typedef struct {
    char name[50];
    char number[10];
} contact;

#endif
