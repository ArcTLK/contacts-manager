#include <stdio.h>
#include <string.h>
#include "manager.h"

int assign(int*);

const int MAX_INPUT_LIMIT = 100;

int main() {
    int mode = 0;
    char input1[MAX_INPUT_LIMIT];
    char input2[MAX_INPUT_LIMIT];
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
            puts("Enter name (max length 50):\t");
            fgets(input1, 50, stdin);
            puts("Enter number:\t");
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
        else if (mode == 2) searchContact();
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
