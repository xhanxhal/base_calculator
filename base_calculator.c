#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_DIGITS 64
#define PRECISION 10


/*
Author: Chanchal Islam
Telegram: @xhanxhal
Number: +8801720942573
Website: https://xhanxhal.github.io/
*/

// Function to convert a character to a digit
int charToDigit(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    return -1;
}

// Function to convert a digit to a character
char digitToChar(int digit) {
    if (digit >= 0 && digit <= 9)
        return '0' + digit;
    else
        return 'A' + (digit - 10);
}

// Function to convert the integer part from any base to decimal
long long baseToDecimalInt(const char *num, int base) {
    long long decimal = 0;
    int len = strlen(num);
    for (int i = 0; i < len; i++) {
        int digit = charToDigit(num[i]);
        if (digit < 0 || digit >= base) {
            printf("Error: Invalid digit '%c' for base %d.\n", num[i], base);
            return -1;
        }
        decimal = decimal * base + digit;
    }
    return decimal;
}

// Function to convert the fractional part from any base to decimal
double baseToDecimalFrac(const char *fracPart, int base) {
    double fraction = 0.0;
    int len = strlen(fracPart);
    for (int i = 0; i < len; i++) {
        int digit = charToDigit(fracPart[i]);
        if (digit < 0 || digit >= base) {
            printf("Error: Invalid digit '%c' in fractional part for base %d.\n", fracPart[i], base);
            return -1.0;
        }
        fraction += digit / pow(base, i + 1);
    }
    return fraction;
}

// Function to convert a decimal integer to any base
void decimalToBaseInt(long long decimal, int base, char *result) {
    int i = 0;
    if (decimal == 0) {
        result[i++] = '0';
    } else {
        while (decimal > 0) {
            int remainder = decimal % base;
            result[i++] = digitToChar(remainder);
            decimal /= base;
        }
    }
    result[i] = '\0';

    // Reverse the result
    for (int j = 0; j < i / 2; j++) {
        char temp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = temp;
    }
}

// Function to convert a decimal fraction to any base
void decimalToBaseFrac(double fraction, int base, char *result) {
    int i = 0;
    while (fraction > 0 && i < PRECISION) {
        fraction *= base;
        int integerPart = (int)fraction;
        result[i++] = digitToChar(integerPart);
        fraction -= integerPart;
    }
    if (i == 0) result[i++] = '0';  // Handle cases where fraction is zero
    result[i] = '\0';
}

// Function to split number into integer and fractional parts
void splitNumber(const char *number, char *intPart, char *fracPart) {
    char *dot = strchr(number, '.');
    if (dot) {
        strncpy(intPart, number, dot - number);
        intPart[dot - number] = '\0';
        strcpy(fracPart, dot + 1);
    } else {
        strcpy(intPart, number);
        fracPart[0] = '\0';
    }
}

// Function to perform arithmetic operations
double performOperation(double num1, double num2, char operator, int *error) {
    *error = 0;
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero.\n");
                *error = 1;
                return 0;
            }
            return num1 / num2;
        default:
            printf("Error: Invalid operator '%c'.\n", operator);
            *error = 1;
            return 0;
    }
}

// Function to validate a base
int isValidBase(int base) {
    return base >= 2 && base <= 36;
}

// Main calculator function with support for fractional parts
void baseCalculator() {
    char num1[MAX_DIGITS], num2[MAX_DIGITS];
    int base1, base2, resultBase;
    char operator;
    int error;

    while (1) {
        printf("\nHello, I\'m Chanchal. Welcome to the Base Calculator with Fraction\n");
        printf("\nEnter first number (integer or fractional): ");
        scanf("%s", num1);

        printf("\nEnter base of first number (2-36): ");
        scanf("%d", &base1);
        if (!isValidBase(base1)) {
            printf("Error: Invalid base %d. Please enter a base between 2 and 36.\n", base1);
            continue;
        }

        printf("\nEnter second number (integer or fractional): ");
        scanf("%s", num2);

        printf("\nEnter base of second number (2-36): ");
        scanf("%d", &base2);
        if (!isValidBase(base2)) {
            printf("Error: Invalid base %d. Please enter a base between 2 and 36.\n", base2);
            continue;
        }

        printf("\nEnter the operation (+, -, *, /): ");
        scanf(" %c", &operator);

        printf("\nEnter the result base (2-36): ");
        scanf("%d", &resultBase);
        if (!isValidBase(resultBase)) {
            printf("Error: Invalid result base %d. Please enter a base between 2 and 36.\n", resultBase);
            continue;
        }

        // Split numbers into integer and fractional parts
        char intPart1[MAX_DIGITS], fracPart1[MAX_DIGITS];
        char intPart2[MAX_DIGITS], fracPart2[MAX_DIGITS];
        splitNumber(num1, intPart1, fracPart1);
        splitNumber(num2, intPart2, fracPart2);

        // Convert integer and fractional parts to decimal
        long long decimalInt1 = baseToDecimalInt(intPart1, base1);
        double decimalFrac1 = baseToDecimalFrac(fracPart1, base1);
        long long decimalInt2 = baseToDecimalInt(intPart2, base2);
        double decimalFrac2 = baseToDecimalFrac(fracPart2, base2);

        if (decimalInt1 == -1 || decimalFrac1 == -1 || decimalInt2 == -1 || decimalFrac2 == -1) {
            printf("Error: Conversion error occurred. Check if numbers match their specified bases.\n");
            continue;
        }

        double decimalNum1 = decimalInt1 + decimalFrac1;
        double decimalNum2 = decimalInt2 + decimalFrac2;

        // Perform operation
        double decimalResult = performOperation(decimalNum1, decimalNum2, operator, &error);
        if (error) continue;

        // Convert result to target base
        char intResult[MAX_DIGITS], fracResult[MAX_DIGITS];
        decimalToBaseInt((long long)decimalResult, resultBase, intResult);
        decimalToBaseFrac(decimalResult - (long long)decimalResult, resultBase, fracResult);

        printf("\n\nResult in base %d: %s.%s\n", resultBase, intResult, fracResult[0] ? fracResult : "0");

        char choice;
        printf("\nDo you want to perform another calculation? (y/n): ");
        scanf(" %c", &choice);
        if (tolower(choice) != 'y') {
            printf("Thank you for using the Base Calculator!\n");
            break;
        }
    }
}

int main() {
    baseCalculator();
    return 0;
}