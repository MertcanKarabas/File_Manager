#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

char * file_list[10];
pthread_t thread_list[5];
pthread_mutex_t mutex;

void fileListControl (char * file_name, int islem) { //islem 0 = add, islem 1 = delete
    for(int i = 0; i< 10; i++) {
        if(islem == 0) {
            if (file_list[i] == NULL) {
                file_list[i] = file_name;
                printf("added.\n");
                break;
            } 
            if (i >= 10) {
                printf("no empty place.\n");
            } 
        } else {
            if(strcmp(file_list[i], file_name) == 0) {
                file_list[i] = NULL;
                printf("deleted.\n");
                break;
            }
            if (i >= 10) {
                printf("wrong file name\n");
            } 
        }
        
     }
}
void create_file(char *file_name) {

    printf("create_file için dosya açılıyor..\n");
    FILE *file = fopen(file_name, "w");
    
    if(file == NULL) {
        perror("error.");
    
    } else {
        //fileListControl(file_name, 0);
        printf("create_file için dosya açıldı..\n");
    }
    fclose(file);
} 

void delete_file(char *file_name) {

    printf("dosya siliniyor..\n");
    if (remove(file_name) == 0){
        printf("dosya silindi..\n");
        printf("file list objesinden silinmesi için gönderildi.\n");
        //fileListControl(file_name, 1);
        printf("Deleted successfully\n");

    } else
        printf("Unable to delete the file\n");
}
void read_file(char *file_name) {
    char data[100];
    printf("read_file için dosya açılıyor..\n");
    FILE *file = fopen(file_name, "r");
    if(file == NULL) {
        perror("error.");
    }else {
        printf("read_file için dosya açıldı..\n");
        while( fgets ( data, 100, file ) != NULL ) {
            // Print the dataToBeRead
            printf( "%s" , data );
        }
        fclose(file);
    }
    
}
void write_file(char *file_name, char * data) {
    printf("write_file için dosya açılıyor..\n");
    FILE *file = fopen(file_name, "w");
    if(file == NULL) {
        perror("error.");
    }else {
        printf("write_file için dosya açıldı..\n");
        if ( strlen ( data ) > 0 ) {
            // writing in the file using fputs()
            printf("write_file data yazılıyor..\n");
            fputs(data, file) ;
            fputs("\n", file) ;
        }    
        fclose(file);
    }
}

void * listen() {

    printf("named_pipe oluşuyor..\n");
    if (mkfifo("file_manager_named_pipe", 0777) == -1) { //fifo is created or not.
        if (errno != EEXIST) { // If fifo is not created
            perror("Could not create fifo file\n");
        }
    }
    printf("named_pipe oluştu..\n");
    int fd;

    while(1){
        
        pthread_mutex_lock(&mutex);
        char input[50];
        char **words;
        words = (char**)malloc(sizeof(char*));
        int i, j, k;
        for (i = 0; i < 10; i++)
            printf("%d. File: %s\n", (i+1), file_list[i]);
        for (i = 0; i < 10; i++)
            words[i] = (char*)malloc(sizeof(char));

        printf("named_pipe okuma için açılıyor...\n");
        fd = open("file_manager_named_pipe", O_RDONLY); 
        if (fd == -1) {
            perror("named_pipe okuma için açılırken hata...\n");
            break;
        }
        printf("named_pipe okuma için açıldı..\n");
        if(read(fd, input, sizeof(input)) == -1) {
            perror("okunurken hata..\n");
            break;
        }
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

        if(strcmp(words[0], "Create") == 0) {
            
            create_file(words[1]);
        } else if(strcmp(words[0], "Delete") == 0){
            
            delete_file(words[1]);
        } else if(strcmp(words[0], "Read") == 0){
            
            read_file(words[1]);
        } else if(strcmp(words[0], "Write") == 0){
            
            write_file(words[1], words[2]);
        } else if(strcmp(words[0], "Exit") == 0){
            
            printf("cikis yapılıyor...");
            break;
        } else {
            printf("yanlis input girdiniz...\n");
        }
        close(fd);

        fd = open("file_manager_named_pipe", O_WRONLY);
        if (fd == -1) {
            perror("named_pipe okuma için açılırken hata...\n");
            break;
        }
        char response[50];
        strcpy(response, words[0]);
        strcat(response, " Done!");
        printf("named_pipe yazma için açıldı..\n");
        if(write(fd, response, sizeof(response)) == -1) {
            printf("yazılırken hata..\n");
            break;
        }
        printf("Comment sent!\n");
        close(fd);
        free(words);
        pthread_mutex_unlock(&mutex);
    }
}

int main () {
    
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 5; i++) {
        if(pthread_create(&thread_list[i], NULL, &listen, NULL) != 0) {
            perror("thread oluşturulurken hata...");
        }
    }
    listen();

    for(int i = 0; i < 5; i++) {
        if(pthread_join(thread_list[i], NULL) != 0) {
            perror("Thread join olurken hata..");
        }
    }
    
    pthread_mutex_destroy(&mutex);

    return 0;
}