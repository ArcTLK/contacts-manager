#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manager.h"

const int MAX_INPUT_LIMIT = 100;
const int MAX_OUTPUT_LIMIT = 1000;

int main() {
    int i;
    char *c;
    char mode = '0';
    char input1[MAX_INPUT_LIMIT];
    char input2[MAX_INPUT_LIMIT];
    char output[MAX_OUTPUT_LIMIT];
    //clean
    memset(input1, 0, sizeof input1);
    memset(input2, 0, sizeof input2);
    memset(output, 0, sizeof output);
    puts("Contacts Manager(Prototype)\t~written by Arc");
    puts(
        "Modes:\n"
        "1: Create a Contact\n"
        "2: Search for a Contact\n"
        "0: Exit Application"
    );
    while(1) { //loop till user breaks
        //get mode from user
        printf("Enter mode:\t");
        mode = getchar();
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
            createContact(input1, input2);
            puts("Contact created!\n");
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
            searchContact(input1, output);
            printf("\n");
            puts(output); //output
            //clean
            memset(input1, 0, sizeof input1);
            memset(output, 0, sizeof output);
        }
        else if (mode == '0') break;
        else printf("Unknown mode. Try again\n");
    }
    return 0;
}
