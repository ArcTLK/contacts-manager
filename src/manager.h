#ifndef _manager_h
#define _manager_h

void createContact();
void searchContact();

typedef struct {
    char name[51];
    char number[11];
} contact;

#endif