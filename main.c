#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manager.h"

int assign(int*);

const int MAX_INPUT_LIMIT = 100;
const int MAX_OUTPUT_LIMIT = 1000;

int main() {
    int mode = 0;
    char input1[MAX_INPUT_LIMIT];
    char input2[MAX_INPUT_LIMIT];
    char output[MAX_OUTPUT_LIMIT];
    //clean
    memset(input1, 0, sizeof(input1));
    memset(input2, 0, sizeof(input2));
    memset(output, 0, sizeof(output));
    puts("Contacts Manager(Prototype)\t~written by Arc");
    puts(
        "Modes:\n"
        "1: Create a Contact\n"
        "2: Search for a Contact\n"
        "6: Exit Application"
    );
    while(1) { //loop till user breaks
        puts("Select a procedure to execute (1 to 6)");
        if (!assign(&mode)) {
            //invalid input!
            puts("Invalid input");
        }
        else if (mode == 1) {
            //get user input
            printf("Enter name (max length 50):\t");
            fgets(input1, 50, stdin);
            printf("Enter number:\t");
            fgets(input2, 15, stdin);
            //remove newline character
            input1[strlen(input1) - 1] = '\0';
            input2[strlen(input2) - 1] = '\0';
            //create contact
            createContact(input1, input2);
            //clean
            memset(input1, 0, sizeof(input1));
            memset(input2, 0, sizeof(input2));
        }
        else if (mode == 2) {
            //get user input
            printf("Enter the name of the person:\t");
            fgets(input1, 50, stdin);
            //remove newline character
            input1[strlen(input1) - 1] = '\0';
            //search contact
            searchContact(input1, output);
            puts(output); //output
            //clean
            memset(input1, 0, sizeof(input1));
            memset(output, 0, sizeof(output));
        }
        else if (mode == 6) break;
        else puts("Unknown mode. Try again");
    }
    return 0;
}

int assign(int *input) {
    char temp[10];
    char *endPtr;
    fgets(temp, 10, stdin);
    *input = strtol(temp, &endPtr, 10);
    return *input;
}
