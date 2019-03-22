#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manager.h"

#define MAX_INPUT_LIMIT 100

void displayList();
void editContactUI();
void deleteContactUI();

int main() {
    int i;
    char *c;
    char mode = '0';
    char input1[MAX_INPUT_LIMIT];
    char input2[MAX_INPUT_LIMIT];
    char *output;
    //clean
    memset(input1, 0, sizeof input1);
    memset(input2, 0, sizeof input2);
    puts("Contacts Manager(Prototype)\t~written by Arc");
    puts(
        "Modes:\n"
        "1: Create a Contact\n"
        "2: Search for a Contact\n"
        "3: Delete a Contact\n"
        "4: View Contacts\n"
        "5: Edit a Contact\n"
        "0: Exit Application"
    );
    while(1) { //loop till user breaks
        //get mode from user
        printf("Enter mode:\t");
        mode = (char)getchar();
        if (mode == '\n') { //handling "no input"
            mode = '0';
            continue;
        }
        else printf("\n");
        //clean input buffer
        while ((i = getchar()) != '\n' && i != EOF);
        if (mode == '1') {
            puts("Contact Creation");
            //get user input
            printf("Enter name:\t");
            fgets(input1, sizeof input1, stdin);
            printf("Enter number:\t");
            fgets(input2, sizeof input2, stdin);
            //remove newline character
            if ((c = strchr(input1, '\n')) != NULL) *c = '\0';
            if ((c = strchr(input2, '\n')) != NULL) *c = '\0';
            //create contact
            if(createContact(input1, input2)) puts("Contact created!\n");
            else puts("Invalid number!\n");
            //clean
            memset(input1, 0, sizeof input1);
            memset(input2, 0, sizeof input2);
        }
        else if (mode == '2') {
            puts("Contact Search");
            //get user input
            printf("Enter the name of the person:\t");
            fgets(input1, sizeof input1, stdin);
            //remove newline character
            if ((c = strchr(input1, '\n')) != NULL) *c = '\0';
            //search contact
            searchContact(input1, &output);
            printf("\n");
            puts(output); //output
            //clean
            memset(input1, 0, sizeof input1);
            free(output);
        }
        else if (mode == '3') {
            deleteContactUI();
        }
        else if (mode == '4') {
            displayList();
        }
        else if (mode == '5') {
            editContactUI();
        }
        else if (mode == '0') break;
        else printf("Unknown mode. Try again\n");
    }
    return 0;
}

void displayList() {
    contact *contacts;
    unsigned int numOfContacts;
    unsigned int i;
    //get contacts
    numOfContacts = readFromFile(&contacts);
    puts("Contact List:");
    for(i = 0; i < numOfContacts; i++) {
        printf("Contact #%d\nName: %s\nNumber: %s\n\n", i + 1, contacts[i].name, contacts[i].number);
    }
    free(contacts);
}
void deleteContactUI() {
    char input[50] = "";
    unsigned int index;
    puts("Deleting a Contact:\nSelect a contact from the contact list.");
    displayList();
    fgets(input, sizeof input, stdin);
    index = (unsigned int)strtol(input, NULL, 10) - 1;
    if(deleteContact(&index)) {
        puts("Contact has been successfully deleted!");
    }
    else puts("An error occured!");
}
void editContactUI() {
    char input[50] = "";
    unsigned int index;
    int option;
    char *c;
    puts("Editing a Contact:\nSelect a contact from the contact list.");
    displayList();
    fgets(input, sizeof input, stdin);
    index = (unsigned int)strtol(input, NULL, 10) - 1;
    //clear
    memset(input, 0, sizeof input);
    puts("Enter 1 for editing name, 2 for editing number");
    fgets(input, sizeof input, stdin);
    option = (int)strtol(input, NULL, 10);
    //clear
    memset(input, 0, sizeof input);
    if (option == 1) printf("Enter new name:\t");
    else if (option == 2) printf("Enter new number:\t");
    fgets(input, sizeof input, stdin);
    //remove newline character
            if ((c = strchr(input, '\n')) != NULL) *c = '\0';
    if (editContact(&index, option, input)) {
        puts("Contact successfully edited!");
    }
    else puts("An error occured!");
}
