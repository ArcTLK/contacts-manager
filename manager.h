#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#define DIR "/storage/emulated/0/contacts.txt"
#define BUFFER_SIZE 100
#define NAME_SIZE 50
#define NUMBER_SIZE 10
#define XML_TAG_SUM 30

typedef struct {
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
} contact;

void createContact(char*, char*);
void searchContact(char*, char**);
int editContact(unsigned int*, int*, char*);
int deleteContact(unsigned int*);

unsigned int readFromFile(contact**);

#endif
