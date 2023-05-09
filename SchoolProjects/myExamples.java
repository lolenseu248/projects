import java.util.Scanner; // import scanner for example number 6 to 10


public class myExamples{
    public static void main(String[] args){
        System.out.println("\n");


        // 1.Adding two numbers
        System.out.print("1. ");
        int num1=23;
        int num2=61;
        int total=num1+num2;
        System.out.println("The sum of 23 and 61 is: "+total);
        System.out.println("\n");


        // 2.Multiplying two numbers
        System.out.print("2. ");
        int number1=5;
        int number2=6;
        int totals=number1*number2;
        System.out.println("The multiplication of 5 by 6 is: "+totals);
        System.out.println("\n");


        // 3.Concatenation of two strings
        System.out.print("3. ");
        String str1="Fruit";
        String str2="Salad";
        // Printing With Concatenation
        System.out.println(str1+" "+str2);
        System.out.println("\n");


        // 4.Make a logo of snail
        System.out.println("4. logo of snail"); // this logo available at https://fsymbols.com/text-art/twitter/
        System.out.println("───▄▄▄");
        System.out.println("─▄▀░▄░▀▄");
        System.out.println("─█░█▄▀░█");  
        System.out.println("─█░▀▄▄▀█▄█▄▀");
        System.out.println("▄▄█▄▄▄▄███▀");
        System.out.println("\n");


        // 5.Get characters on array
        System.out.println("5. Get characters on string Hello World using indexing");
        String text=""+"Hello World".charAt(6); // get the index number of 6 (counting starts from 0)
        System.out.println("The index number 6 from string Hello World is: "+text);
        System.out.println("\n");



        // 6 to 10 will need user input to run a basic program

        // 6.Full Name
        System.out.println("6. Input Name ");
        System.out.print("Enter your Full Name (LastName First): ");
        Scanner scanName = new Scanner(System.in);  // object for name
        String userName = scanName.nextLine();  // user input
        System.out.print("Hello "+userName);
        System.out.println("\n");


        // 7.Address
        System.out.println("7. Input Address");
        System.out.print("Enter your Address: ");
        Scanner scanAddress = new Scanner(System.in);  // object for addresss
        String userAddress = scanAddress.nextLine();  // user input
        System.out.print("You are from "+userAddress);
        System.out.println("\n");


        // 8. Gender, Age, and Birthday
        System.out.println("8. Input Gender, Age, and Birthday");
        System.out.print("Enter your Gender (Male/Female): ");
        Scanner scanGender = new Scanner(System.in);  // object for gender
        String userGender = scanGender.nextLine();  // user input
        System.out.println("Your gender "+userGender);
        System.out.println("\n");

        System.out.print("Enter your Age: ");
        Scanner scanAge = new Scanner(System.in);  // object for age
        String userAge = scanAge.nextLine();  // user input
        System.out.println("You are Age "+userAge);
        System.out.println("\n");

        System.out.print("Enter your Birthday: ");
        Scanner scanBday = new Scanner(System.in);  // object for bday
        String userBday = scanBday.nextLine();  // user input
        System.out.println("Your Birthday "+userBday);
        System.out.println("\n");


        // 9.BMI (Body Mass Index) Calculator
        System.out.println("9. BMI calculator");
        System.out.print("Enter your in Feet Example 5.5: "); // user Height in feet
        Scanner scanFeet = new Scanner(System.in);
        float heightInFeets = scanFeet.nextFloat();
        System.out.print("Enter your Weight in Kilogram Example 60.4: "); // user Weight in kg
        Scanner scanWeight = new Scanner(System.in);
        float weightInKG = scanWeight.nextFloat(); 

        // formula on https://www.calculatorsoup.com/calculators/health/bmi-calculator.php
        float squareOfFeet = heightInFeets*heightInFeets;
        float totalBMI = weightInKG/squareOfFeet;
        System.out.printf("Your BMI is %.2f",totalBMI); // print the bmi
        System.out.println("\n");
        
 
        // 10. Bootstrap all data and print
        System.out.println("\n10. Printing all Details\n");
        System.out.println("Details: ");
        System.out.println("Name: Mr./Ms. "+userName);
        System.out.println("Address: "+userAddress);
        System.out.println("Age: "+userAge+" Years Old");
        System.out.println("Birthday: "+userBday);
        System.out.println("Gender: "+userGender);
        System.out.printf("BMI is %.2f",totalBMI);
        System.out.println("");

        // print BMI status
        System.out.print("BMI Status: ");
        if(totalBMI<18.8)System.out.println("Underweight");
        else if(totalBMI<20.8)System.out.println("Normal");
        else if(totalBMI<40.0)System.out.println("Overweight");
        else System.out.println("Obese");
        System.out.println("\n");
    }
}
