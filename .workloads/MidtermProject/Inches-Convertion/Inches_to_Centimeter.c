/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program converts and prints the value of inches to centimeter.
    
    Output:
        Inches to Centimeter
        Enter a Value of Inches: 1
        Answer: 2.540000

*/

#include <stdio.h>

int main() {
    float myNumber;
    float answer;

    printf("Inches to Centimeter\n");
    printf("Enter a Value of Inches: ");
    scanf("%f", &myNumber);
    answer = myNumber * 2.54; // input value multiply to 2.54
    printf("Answer: %f\n", answer);

}
