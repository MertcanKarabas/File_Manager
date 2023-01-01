#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

int main () {
    int fd;
    while(1) {
        char input[50];
        scanf("%s", input);
        printf("named_pipe yazma için açılıyor..\n");
        fd = open("file_manager_named_pipe", O_WRONLY);
        if (fd == -1) {
            printf("named_pipe açılırken hata..\n");
            return 1;
        }
        printf("named_pipe yazma için açıldı..\n");
        if(write(fd, input, sizeof(input)) == -1) {
            printf("yazılırken hata..\n");
            return 2;
        }
        printf("%s", input);
        
        int i = 0;
        if (i == 3) {
            return 3;
        }
        i++;
    }
    close(fd);
    return 0;
}