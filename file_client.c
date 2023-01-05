#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

int main () {
    int fd;
    
    while(1) {
        char input[50];
        char response[50];
        int i, j, k;
        fgets(input, 50, stdin);  // Kullanıcıdan bir string alınır
        char * words[10];
        char * token = strtok(input, " "); // strtok fonksiyonu kullanılarak cümle kelimelere ayrılır
        i = 0;
        while (token != NULL) { // kelime NULL olana kadar döngü
            words[i++] = token;
            token = strtok(NULL, " "); // sonraki kelime alınır
        }

        fd = open("file_manager_named_pipe", O_WRONLY);
        if (fd == -1) {
            printf("named_pipe açılırken hata..\n");
            return 1;
        }

        printf("named_pipe yazma için açıldı..\n");
        if(write(fd, &words, sizeof(input)) == -1) {
            printf("yazılırken hata..\n");
            return 2;
        }
        printf("Comment sent!\n");


        if(strcmp(words[0], "Exit") == 0) {
            printf("Çıkış yapılıyor..\n");
            break;
        }
        close(fd);

        printf("named_pipe okuma için açılıyor...\n");
        fd = open("file_manager_named_pipe", O_RDONLY);    
        if (fd == -1) {
            perror("named_pipe okuma için açılırken hata...\n");
            break;
        }
        printf("named_pipe okuma için açıldı..\n");
        if(read(fd, response, sizeof(response)) == -1) {
            perror("okunurken hata..\n");
            break;
        }
        printf("%s\n", response);
        close(fd);       
    }
    printf("pipe kapanıyor..\n");
    
    printf("pipe kapandı..\n");
    printf("çıkış yapıldı...\n");
    return 0;
}