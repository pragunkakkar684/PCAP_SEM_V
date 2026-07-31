#include <stdio.h>

int main() {
    int a[9], rev[9];
    int i, n, r, reverse;

    printf("Enter 9 integers:\n");

   // Read
    for (i = 0; i < 9; i++) {
        scanf("%d", &a[i]);
    }

    // Reverse the digits 
    for (i = 0; i < 9; i++) {
        n = a[i];
        reverse = 0;

        while (n != 0) {
            r = n % 10;
            reverse = reverse * 10 + r;
            n = n / 10;
        }

        rev[i] = reverse;
    }

    // Display 
    printf("\nReversed digits:\n");
    for (i = 0; i < 9; i++) {
        printf("%d ", rev[i]);
    }

    return 0;
}
