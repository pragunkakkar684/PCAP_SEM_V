#include <stdio.h>
#include <string.h>

int main() {
    char s1[100], s2[100], result[200];
    int i = 0, j = 0;

    printf("Enter string S1: ");
    scanf("%s", s1);
    
    printf("Enter string S2: ");
    scanf("%s", s2);

    while (s1[i] != '\0' && s2[i] != '\0') {
        result[j++] = s1[i]; 
        result[j++] = s2[i]; 
        i++;
    }

    result[j] = '\0';

    printf("Resultant string: %s\n", result);

    return 0;
}
