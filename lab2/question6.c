#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

int main() {
    char str[100];
    int n, i;

    printf("Pragun Kakkar - 240968332\n");
    printf("Enter a string: ");
    scanf("%s", str);

    n = strlen(str);

    // Create one thread for each character
    #pragma omp parallel for num_threads(n)
    for (i = 0; i < n; i++) {
        if (islower(str[i]))
            str[i] = toupper(str[i]);
        else if (isupper(str[i]))
            str[i] = tolower(str[i]);

        printf("Thread %d toggled index %d\n",
               omp_get_thread_num(), i);
    }

    printf("\nToggled String: %s\n", str);

    return 0;
}