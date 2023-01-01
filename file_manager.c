#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

int main () {

    printf("named_pipe oluşuyor..\n");
    if (mkfifo("file_manager_named_pipe", 0777) == -1) { //fifo is created or not.
        if (errno != EEXIST) { // If fifo is not created
        printf ("Could not create fifo file\n");
            return 1;
        }
    }
    printf("named_pipe oluştu..\n");
    int fd;
    while(1){
        char input[50];
        char * fileList[10];
        printf("named_pipe okuma için açılıyor...\n");
        fd = open("file_manager_named_pipe", O_RDONLY);    
        if (fd == -1) {
            printf("named_pipe okuma için açılırken hata...\n");
            return 2;
        }
        printf("named_pipe okuma için açıldı..\n");
        if(read(fd, input, sizeof(input)) == -1) {
            printf("okunurken hata..\n");
            return 3;
        }
        printf("%s\n", input);
        printf("named_pipe kapatılıyor..\n");
        printf("named_pipe kapatıldı..\n");
        int i = 0;
        if(i == 3) {
            return 4;
        }
        i++;
    }
    close(fd);
    return 0;
}