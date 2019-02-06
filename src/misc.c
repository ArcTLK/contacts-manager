#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

void assign(int *input) {
    char temp[10];
    char *endPtr;
    fgets(temp, 10, stdin);
    *input = strtol(temp, &endPtr, 10);
    if (*input == 0) {
        puts("Enter a valid integer value between 1-5!\n");
    }
}