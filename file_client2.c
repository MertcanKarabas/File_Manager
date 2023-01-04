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
        char **words;
        int i, j, k;
        fgets(input, 50, stdin);  // Kullanıcıdan bir string alınır

        words = (char**)malloc(sizeof(char*));
        for (i = 0; i < 10; i++)
            words[i] = (char*)malloc(sizeof(char));

        input[strlen(input) -1 ] = '\0'; //enter karakterini sil.
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
        printf("Comment sent!\n");

        i = 0; j = 0;
        int len = strlen(input);
        // "command" array'i içerisinde kelime kelime ayrıştırılır
        for (k = 0; k < len; k++) {
            // Eğer kelime sonuna gelinmişse, j'yi sıfırla ve i'yi bir arttır
            if (input[k] == ' ' || input[k] == '\0' || input[k] == '\n') {
                words[i][j] = '\0';
                i++;
                j = 0;
            } else {
            // Aksi halde, kelimeyi "words" array'ine at
                words[i][j] = input[k];
                j++;
            }
        }

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
        free(words);       
    }
    printf("pipe kapanıyor..\n");
    
    printf("pipe kapandı..\n");
    printf("çıkış yapıldı...\n");
    return 0;
}