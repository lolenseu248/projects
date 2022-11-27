/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program converts and prints the value of inches to feet, yard, centimeters, and meter.
        a. Convert inches to feet
        b. Convert inches to yard
        c. Convert inches to centimeters
        d. Convert inches to meter

    Output:
        Enter:
        1 to convert Inches to Feet.
        2 to convert Inches to Yard.
        3 to convert Inches to centimeters.
        4 to convert Inches to Meter.
        5 to convert Inches to All.
        Your Chose: 1

        Inches to Feet
        Enter a Value of Inches: 1
        Inches to Feet is: 0.083333
        
*/

#include <stdio.h>
#include <stdlib.h> // include library for exit(1)

int main() {
    int counter = 0; // counter
    int myChoice;    // container for choice
    
    // the following line will serve as a containers for input value and answers
    float myNumber;
    float answer;
    float answer1;
    float answer2;
    float answer3;
    float answer4;

    while (1) { // loop for continues convertion
        counter += 1;
        if (counter == 2) { // if the counter reach 2 auto exit means you enter a wrong option/choice
            exit(1);
        }

        printf("\nEnter:\n1 to convert Inches to Feet.\n2 to convert Inches to Yard.\n3 to convert Inches to Centimeters.\n4 to convert Inches to Meter.\n5 to convert Inches to All\n");
        printf("Your Choice: ");
        scanf("%d", &myChoice);

        // the following line will be triggerd if the choice are correct
        // else Wrong Choice! auto exit
        if (myChoice == 1) {
            printf("\nInches to Feet\n");
            printf("Enter a Value of Inches: ");
            scanf("%f", &myNumber);
            answer = myNumber / 12.0; // input value divided to 12.0
            printf("Inches to Feet is: %f\n", answer);
            counter = 0;
        }

        else if (myChoice == 2) {
            printf("\nInches to Yard\n");
            printf("Enter a Value of Inches: ");
            scanf("%f", &myNumber);
            answer = myNumber / 36.0; // input value divided to 36.0
            printf("Inches to Yard is: %f\n", answer);
            counter = 0;
        }

        else if (myChoice == 3) {
            printf("\nInches to Centimeter\n");
            printf("Enter a Value of Inches: ");
            scanf("%f", &myNumber);
            answer = myNumber * 2.54; // input value multiply to 2.54
            printf("Inches to Centimeter is: %f\n", answer);
            counter = 0;
        }

        else if (myChoice == 4) {
            printf("\nInches to Meter\n");
            printf("Enter a Value of Inches: ");
            scanf("%f", &myNumber);
            answer = myNumber / 39.37; // input value divided to 39.37
            printf("Inches to Meter is: %f\n", answer);
            counter = 0;
        }
        
        else if (myChoice == 5) {
            printf("\nInches to All\n");
            printf("Enter a Value of Inches: ");
            scanf("%f", &myNumber);
            answer1 = myNumber / 12.0; // input value divided to 12.0
            answer2 = myNumber / 36.0; // input value divided to 36.0
            answer3 = myNumber * 2.54; // input value multiply to 2.54
            answer4 = myNumber / 39.37; // input value divided to 39.37
            printf("Inches to Feet is: %f\nInches to Yard is: %f\nInches to Centimeter is: %f\nInches to Meter is: %f\n", answer1, answer2, answer3, answer4);
            counter = 0;
         }
         
        // the counter auto reset to 0 if the choice are correct

        else {
            printf("Wrong Choice!\n"); // auto exit if wrong choice
            //re-run the program to continue

        }

    }
}
