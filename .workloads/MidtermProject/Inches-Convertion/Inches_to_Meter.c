/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program converts and prints the value of inches to meter.
    
    Output:
        Inches to Meter
        Enter a Value of Inches: 1
        Answer: 0.025400

*/

#include <stdio.h>

int main() {
    float myNumber;
    float answer;

    printf("Inches to Meter\n");
    printf("Enter a Value of Inches: ");
    scanf("%f", &myNumber);
    answer = myNumber / 39.37; // input value divided to 39.37
    printf("Answer: %f\n", answer);

}
