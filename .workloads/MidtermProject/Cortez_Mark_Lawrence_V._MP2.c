/*
    Written by: Mark Lawrence V. Cortez
    Date: Nov. 26, 2022
    Yr&Sec: 2022-2023 BSIT-1D

    This program calculate the cost per kilometer and print the total cost/value.

    Output:
        Cost per Kilometer is: 81.000000
        Enter a Value of Kilomiter: 1
        Total cost is: 81

*/

#include <stdio.h>

int main () {
    float pesos = 20.25;
    int meter = 250;

    int myNumber;
    int answer;

    float calMeter = 1000 / meter; //1000 meter equal to 1 kilometer and divided to 250 = 4
    float cost_per_kilometer = pesos * calMeter; // 20.25 multiply to 4 = 81

    printf("Cost per Kilometer is: %f\n", cost_per_kilometer);
    printf("Enter a Value of Kilomiter: ");
    scanf("%d", &myNumber);
    answer = cost_per_kilometer * myNumber; // 81 multply to input value
    printf("Total cost is: %d\n", answer);

}
