#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//oled screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino resetpin)

//screen initiation
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//joystic pinoutss
#define joyX1 26
#define joyY1 25
#define joySW1 35

#define joyX2 32
#define joyY2 33
#define joySW2 34

//potentiometer pinouts
#define potenMeter1 14
#define potenMeter2 12

//toggle switchs
#define togSW1 4
#define togSW2 0
#define togSW3 2
#define togSW4 15


//variables
//raw data
//joystic inputs
int joyX1Pos;
int joyY1Pos;
int joySW1State;

int joyX2Pos;
int joyY2Pos;
int joySW2State;

//potentiometer inputs
int potenM1Pos;
int potenM2Pos;

//toggle
int togSW1State;
int togSW2State;
int togSW3State;
int togSW4State;

//mapped data
//joystic maps
int joyX1Poss;
int joyY1Poss;

int joyX2Poss;
int joyY2Poss;


//potentiometer maps
int potenM1Poss;
int potenM2Poss;



//potentiometer map voltage
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//counter
int counter = 0;



void setup() {
  // put your setup code here, to run once:

  //initiation 
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  //joystick switch
  pinMode(joySW1, INPUT);
  pinMode(joySW2, INPUT);

  //toggle switch
  pinMode(togSW1, INPUT_PULLUP);
  pinMode(togSW2, INPUT_PULLUP);
  pinMode(togSW3, INPUT_PULLUP);
  pinMode(togSW4, INPUT_PULLUP);


  //display
  display.clearDisplay();
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

  //counter
  if (counter == 100) {
    counter = 0;
  }
  counter += 1;
  
  //raw data
  //read X,Y and SW analog values of joystic no.1
  joyX1Pos = analogRead(joyX1);
  joyY1Pos = analogRead(joyY1);
  joySW1State = digitalRead(joySW1);

  //read X,Y and SW analog values of joystic no.2
  joyX2Pos = analogRead(joyX2);
  joyY2Pos = analogRead(joyY2);
  joySW2State = digitalRead(joySW2);

  //read potentiometer analog values
  potenM1Pos = analogRead(potenMeter1);
  potenM2Pos = analogRead(potenMeter2);

  //read toglle input value
  togSW1State = digitalRead(togSW1);
  togSW2State = digitalRead(togSW2);
  togSW3State = digitalRead(togSW3);
  togSW4State = digitalRead(togSW4);


  //mapped data
  //mapped joystic values of joystic no.1
  joyX1Poss = map(joyX1Pos, 1000, 2000, 0, 255);
  joyY1Poss = map(joyY1Pos, 1000, 2000, 0, 255);

  //mapped joystic values of joystic no.1
  joyX2Poss = map(joyX2Pos, 1000, 2000, 0, 255);
  joyY2Poss = map(joyY2Pos, 1000, 2000, 0, 255);

  //mapped potentiometer
  potenM1Poss = map(potenM1Pos, 1000, 2000, 0, 255);
  potenM2Poss = map(potenM2Pos, 1000, 2000, 0, 255);
  


  //debug
  Serial.printf("Raw Data \n");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d \n",joyX1Pos,joyY1Pos,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d \n",joyX2Pos,joyY2Pos,joySW2State);
  Serial.printf("PotentioMeter no.1= %f \n",potenM2Pos);
  Serial.printf("PotentioMeter no.2= %f \n",potenM2Pos);
  Serial.printf("\n");
  
  Serial.printf("Mapped Data \n");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d \n",joyX1Poss,joyY1Poss,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d \n",joyX2Poss,joyY2Poss,joySW2State);
  Serial.printf("PotentioMeter no.1= %f \n",potenM2Poss);
  Serial.printf("PotentioMeter no.2= %f \n",potenM2Poss);
  Serial.printf("\n");
  
  Serial.printf("Switch \n");
  Serial.printf("JoyStick no.1= %d \n",joySW1State);
  Serial.printf("JoyStick no.2= %d \n",joySW2State);
  Serial.printf("Toggle no.1= %d \n",togSW1State);
  Serial.printf("Toggle no.2= %d \n",togSW2State);
  Serial.printf("Toggle no.3= %d \n",togSW3State);
  Serial.printf("Toggle no.4= %d \n",togSW4State);
  Serial.printf("\n");
  
  Serial.printf("Counter= %d \n",counter);


  //oled screen debug
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.print("JSNo.1: ");
  display.print("\t=");
  display.print(joyX1Poss);
  display.print("\tY=");
  display.print(joyY1Poss);
  
  display.setCursor(0, 10);
  display.print("JSNo.2: ");
  display.print("\tX=");
  display.print(joyX2Poss);
  display.print("\tY=");
  display.print(joyY2Poss);

  display.setCursor(0, 20);
  display.print("PMNo.1: =");
  display.print(potenM1Poss);

  display.setCursor(0, 30);
  display.print("PMNo.2: =");
  display.print(potenM2Poss);
  
  display.setCursor(0, 100);
  display.print("Counter: ");
  display.print(counter);
  
  display.display();



  //delay
  delay(100);
}
