/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program converts and prints the value of inches to feet.
    
    Output:
        Inches to Feet
        Enter a Value of Inches: 1
        Answer: 0.083333

*/

#include <stdio.h>

int main() {
    float myNumber;
    float answer;

    printf("Inches to Feet\n");
    printf("Enter a Value of Inches: ");
    scanf("%f", &myNumber);
    answer = myNumber / 12.0; // input value divided to 12.0
    printf("Answer: %f\n", answer);

}
