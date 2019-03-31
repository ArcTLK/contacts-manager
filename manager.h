#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#define DIR "./contacts.txt"
#define BUFFER_SIZE 100
#define NAME_SIZE 50
#define NUMBER_SIZE 12
#define XML_TAG_SUM 30
//error codes
#define ERR_NAN 1000
#define ERR_NUM_LENGTH_MISMATCH 1001
#define ERR_INVALID_PROPERTY 1002
#define ERR_INVALID_INDEX 1003
#define ERR_DUPLICATE_CONTACT 1004

typedef struct {
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
} contact;

int createContact(char*, char*);
int searchContact(char*, int, contact**, unsigned int*);
int editContact(unsigned int*, int, char*);
int deleteContact(unsigned int*);

unsigned int readFromFile(contact**);

#endif
