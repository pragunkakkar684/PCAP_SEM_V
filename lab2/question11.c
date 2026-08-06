#include <stdio.h>
#include <omp.h>

// Function to reverse the digits of a given integer
int reverse_digits(int n) {
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + (n % 10);
        n /= 10;
    }
    return reversed;
}

int main() {
    int size = 9;
    int input[9] = {18, 523, 301, 1234, 2, 14, 108, 150, 1928};
    int output[9];
    printf("Pragun kakkar - 240968332\n");

    // Parallelize the loop using OpenMP
    #pragma omp parallel for default(none) shared(input, output, size)
    for (int i = 0; i < size; i++) {
        output[i] = reverse_digits(input[i]);
    }

    // Display the results
    printf("Input Array:  ");
    for (int i = 0; i < size; i++) {
        printf("%d ", input[i]);
    }
    printf("\n");

    printf("Output Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");

    return 0;
}