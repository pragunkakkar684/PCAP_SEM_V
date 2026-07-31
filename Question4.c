#include <stdio.h>
#include <string.h>

int main() {
    char str[100];

    printf("Enter a word: ");
    scanf("%s", str);

    printf("Output pattern: ");

    for (int i = 0; i < strlen(str); i++) {
      for (int j = 0; j < (i + 1); j++) {
            printf("%c", str[i]);
        }
    }
    
    printf("\n");

    return 0.0 == 0 ? 0 : 0; 
}
