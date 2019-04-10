#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manager.h"

void displayList();
void editContactUI();
void deleteContactUI();
void searchContactUI();
void createContactUI();

int main() {
    int i;
    char mode = '0';
    puts("Contacts Manager");
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

        if (mode == '1') createContactUI();
        else if (mode == '2') searchContactUI();
        else if (mode == '3') deleteContactUI();
        else if (mode == '4') displayList();
        else if (mode == '5') editContactUI();
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
    char input[BUFFER_SIZE] = "";
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
    char input[BUFFER_SIZE] = "";
    unsigned int index;
    int option;
    char *c;
    int errorId;
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
    errorId = editContact(&index, option, input);
    if (errorId == ERR_NAN) puts("Number must be a numerical value!\n");
    else if (errorId == ERR_DUPLICATE_CONTACT) puts("A contact with the same number is present in the list!\n");
    else if (errorId == ERR_INVALID_PROPERTY) puts("Invalid property entered!\n");
    else if (errorId == ERR_INVALID_INDEX) puts("Invalid contact selected!\n");
    else puts("Contact has been edited!\n");
}

void searchContactUI() {
    char name[NAME_SIZE] = "";
    char *c;
    contact *foundContacts;
    unsigned int contactsFound;
    int i;
    puts("Contact Search");
    //get user input
    printf("Enter the name of the person:\t");
    fgets(name, sizeof name, stdin);
    //remove newline character
    if ((c = strchr(name, '\n')) != NULL) *c = '\0';
    //search contact
    searchContact(name, 0, &foundContacts, &contactsFound);
    if (contactsFound > 0) {
        printf("Name: %s\nNumbers:\n", name);
        for (i = 0; i < contactsFound; i++) {
            printf("%s\n", foundContacts[i].number);
        }
    }
    else printf("No contacts found!\n");
    printf("\n");
    //clean
    free(foundContacts);
}

void createContactUI() {
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
    int errorId;
    char *c;
    puts("Contact Creation");
    //get user input
    printf("Enter name:\t");
    fgets(name, sizeof name, stdin);
    printf("Enter number:\t");
    fgets(number, sizeof number, stdin);
    //remove newline character
    if ((c = strchr(name, '\n')) != NULL) *c = '\0';
    if ((c = strchr(number, '\n')) != NULL) *c = '\0';
    //create contact
    errorId = createContact(name, number);
    if (errorId == ERR_NAN) puts("Number must be a numerical value!\n");
    else if (errorId == ERR_DUPLICATE_CONTACT) puts("A contact with the same number is present in the list!\n");
    else puts("Contact has been created!\n");
}
