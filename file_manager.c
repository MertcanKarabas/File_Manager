#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

    while(1) {

        char input[50];
        fgets(input, 50, stdin);
        char *split = NULL;
        split = strtok(input, " ");
        char *inputs[100];
        int counter = 0;
        while(split != NULL) {
            inputs[counter] = split;
            split = strtok(NULL, " ");
            counter++;
        }

        if ((strcmp(inputs[0], "create\n") == 0) ){
            printf("Create calisti\n");
            

        } else if((strcmp(inputs[0], "delete\n") == 0)) {
            printf("delete calisti\n");

        } else if((strcmp(inputs[0], "read\n") == 0)) {
            printf("read calisti\n");

        } else if(strcmp(inputs[0], "write\n") == 0) {
            printf("write calisti\n");

        } else if (strcmp(inputs[0], "exit\n")) {
            return 0;

        } else if ((strcmp(inputs[0], "clear\n") == 0)) {
            printf("\e[1;1H\e[2J");

        } else {
            printf("You gave wrong input.\n");
        }
    }


    return 0;
}