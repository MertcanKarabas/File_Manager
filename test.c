#include <stdio.h>
#include <string.h>

int main(void) {
  char sentence[100] = "sa as nbr ii nbr sa"; // char dizisi oluşturulur
  printf("Enter a sentence: ");
  //fgets(sentence, 100, stdin); // Kullanıcıdan cümle alınır
  char *words[10];
  char * token = strtok(sentence, " "); // strtok fonksiyonu kullanılarak cümle kelimelere ayrılır
  int i = 0;
  while (token != NULL) { // kelime NULL olana kadar döngü
    words[i++] = token;
    token = strtok(NULL, " "); // sonraki kelime alınır
  }
  int j = 0;
  while(words[j] != NULL) {
    printf("LWORD: %s\n", words[j++]);
  }
  return 0;
}