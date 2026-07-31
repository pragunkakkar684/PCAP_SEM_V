#include <stdio.h>

int main() {
 
    float a, b;
    char op;

    printf("Enter an operator (+, -, *, /): ");
    scanf("%c", &op);

    printf("Enter number A: ");
    scanf("%f", &a);
    
    printf("Enter number B: ");
    scanf("%f", &b);

    if (op == '+') {
        printf("Result: %.2f + %.2f = %.2f\n", a, b, a + b);
    } 
    else if (op == '-') {
        printf("Result: %.2f - %.2f = %.2f\n", a, b, a - b);
    } 
    else if (op == '*') {
        printf("Result: %.2f * %.2f = %.2f\n", a, b, a * b);
    } 
    else if (op == '/') {
        if (b == 0) {
            printf("Error! Division by zero is not allowed.\n");
        } else {
            printf("Result: %.2f / %.2f = %.2f\n", a, b, a / b);
        }
    } 
    else {
        printf("Error! Invalid operator.\n");
    }

    return 0;
}
