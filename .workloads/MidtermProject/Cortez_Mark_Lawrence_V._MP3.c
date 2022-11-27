/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program

    Output:
        Enter a amount of milk produced in the morning: 10
        The total carton needed to hold the milk is 3,
        Total cost for producing per liter is $4,
        And that total profit for producing is $3.

*/

#include <stdio.h>

int main() {
    float carton_liters_of_milk = 3.78;
    float cost_liter_of_milk = 0.38;
    float profit_carton_of_milk = 0.27;

    // the following line will store the input value and anwers
    float myNumber;
    float anwser1;
    float anwser2;
    float anwser3;

    printf("Enter a amount of milk produced in the morning: ");
    scanf("%f", &myNumber);
    anwser1 = myNumber / carton_liters_of_milk; // input value divided to carton liters of milk
    anwser2 = myNumber * cost_liter_of_milk; // input value multipy to cost liter of milk
    anwser3 = myNumber * profit_carton_of_milk; // input value multipy to profit carton of milk

    /*
        The following line will print the 
        total carton needed to hold the milk,
        total cost for production per liter,
        and profit per production.
    */
    printf("The total carton needed to hold the milk is %.0f,\nTotal cost for producing per liter is $%.0f,\nAnd that total profit for producing is $%.0f.\n", anwser1, anwser2, anwser3);

}
