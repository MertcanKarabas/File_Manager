#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> //this is for open, close 

char * file_list[10];
pthread_t thread_list[5];
pthread_mutex_t mutex;

bool fileListControl (char * file_name, int islem) { //islem 0 = ekleme, islem 1 = çıkarma

    bool res = false;

    for(int i = 0; i< 10; i++) { //file list içini kontrol etmek için döngü

        if(islem == 0) { //ekleme

            bool oDosyaVarMi = false;
            for (int j = 0; j < 10; j++) { //aynı isimde dosya olup olmadığını kontrol eden döngü
                if (file_list[i] == NULL) {
                    continue;
                } else if(strcmp(file_list[i], file_name) == 0) {
                    oDosyaVarMi = true;
                }
            }

            if (oDosyaVarMi) {
                printf("ayni isimde dosya olduğundan oluşturulamadı..\n");
                return res;

            } else if(file_list[i] == NULL) {
                file_list[i] = file_name;
                printf("added.\n");
                res = true;
                return res;
            }
            if (i >= 10) {
                printf("no empty place.\n");
                return res;
            } 

        } else { //çıkarma - silme

            if(file_list[i] == NULL) {
                continue;

            } else if(strcmp(file_list[i], file_name) == 0) {
                file_list[i] = NULL;
                printf("deleted.\n");
                res = true;
                return res;
            }

            if (i >= 10) {
                printf("wrong file name\n");
                return res;;
            } 
        }
    }
    printf("bir şeyler ters gitti ve dosya işlemin başarılı olmadı..\n");
    return res;
}

void create_file(char *file_name) { //file liste ekleme ve dosyayı oluşturma

    if(fileListControl(file_name, 0)) {

        printf("create_file için dosya açılıyor..\n");
        FILE *file = fopen(file_name, "w");
    
        if(file == NULL) {
            perror("error.");
    
        } else {
        
            printf("create_file için dosya açıldı..\n");
        }
        fclose(file);

    } else {
        printf("file oluşturulamadı..");
    }
    
} 
void delete_file(char *file_name) { //file listten ve sistemden dosyayı silme

    if(fileListControl(file_name, 1)){
         printf("dosya siliniyor..\n");
        if (remove(file_name) == 0){
            printf("dosya silindi..\n");

        } else
            printf("Unable to delete the file\n");

    } else {
        printf("file silinemedi..");
    }
   
}
void read_file(char *file_name) { //dosyayı okuma

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
void write_file(char *file_name, char * data) { //dosyanın içine veriyi yazma

    printf("write_file için dosya açılıyor..\n");

    FILE *file = fopen(file_name, "a");
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

void namedPipeOlustur() {

    printf("named_pipe oluşuyor..\n");
    if (mkfifo("file_manager_named_pipe", 0777) == -1) { //fifo is created or not.
        if (errno != EEXIST) { // If fifo is not created
            perror("Could not create fifo file\n");
        }
    }
    printf("named_pipe oluştu..\n");
}

void * listen() {

    namedPipeOlustur();
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