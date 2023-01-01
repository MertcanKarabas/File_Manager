#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

int main () {

    if (mkfifo("file_manager_named_pipe", 0777) == -1) { //fifo is created or not.
        if (errno != EEXIST) { // If fifo is not created
        printf ("Could not create fifo file\n");
            return 1;
        }
    }
    int fd = open("file_manager_named_pipe", O_WRONLY);    
    if (fd == -1) {
        return 2;
    }
    
    return 0;
}