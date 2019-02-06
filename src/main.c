#include <stdio.h>
#include "misc.h"
#include "manager.h"

int main() {
    int mode = 0;
    puts("Contacts Manager\t~written by Arc");
    while(1) {
        if (mode == 0) {
            puts(
                "Modes:\n"
                "1: Create a Contact"
            );
        }
        else if (mode == 1) {
            createContact();
        }
        else if (mode == 5) break;
        else {
            puts("Unknown mode. Try again");
        }
        assign(&mode);
    }
    return 0;
}