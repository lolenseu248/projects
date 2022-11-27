/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program will print the pounds value from kilogram.

    Output:
        Enter your Weight in Kilogram: 64
        Your Weight in Kilogram is 64.00 and in Pounds is 141.12

*/

#include <stdio.h>

int main() {
    float pounds = 2.205;

    float myNumber;
    float answer;

    printf("Enter your Weight in Kilogram: ");
    scanf("%f", &myNumber);
    answer = myNumber * pounds; // input value multiply to pounds which is 2.205
    printf("Your Weight in Kilogram is %.2f and in Pounds is %.2f\n", myNumber, answer); // printing the value of kilogram and pounds raoud off

}
