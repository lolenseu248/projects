#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Joystic Pinouts
int joyX1=19;
int joyY1=18;
int joySW1=5;

int joyX2=17;
int joyY2=16;
int joySW2=4;

//Potentiometer Pinouts
int potenMeter1=0;
int potenMeter2=2;

//Toggle Switchs
int togSW1=35;
int togSW2=34;
int togSW3=39;
int togSW4=36;


//Variables
//Joystic Inputs and Maps
int joyX1Pos;
int joyY1Pos;
int joySW1State;

int joy_mapX1;
int joy_mapY1;

int joyX2Pos;
int joyY2Pos;
int joySW2State;

int joy_mapX2;
int joy_mapY2;


//Potentiometer Inputs and Maps
int potenM1Pos;
int potenM2Pos;


char msg[32];


void setup() {
  Serial.begin(115200);
  
  pinMode(joyX1,INPUT);
  pinMode(joyY1,INPUT);
  pinMode(joySW1,INPUT_PULLUP);
  
  pinMode(joyX2,INPUT);
  pinMode(joyY2,INPUT);
  pinMode(joySW2,INPUT_PULLUP);

  pinMode(potenMeter1,INPUT);
  pinMode(potenMeter2,INPUT);

  pinMode(togSW1,INPUT);
  pinMode(togSW2,INPUT);
  pinMode(togSW3,INPUT);
  pinMode(togSW4,INPUT);
}


void loop() {
  //Proccessing the inputs data
  joyX1Pos=analogRead(joyX1);
  joyY1Pos=analogRead(joyY1);
  joySW1State=digitalRead(joySW1);

  joy_mapX1=map(joyX1Pos,0,1023,1000,2000);
  joy_mapY1=map(joyY1Pos,0,1023,1000,2000);

  joyX2Pos=analogRead(joyX2);
  joyY2Pos=analogRead(joyY2);
  joySW2State=digitalRead(joySW2);

  joy_mapX2=map(joyX2Pos,0,1023,1000,2000);
  joy_mapY2=map(joyY2Pos,0,1023,1000,2000);

  potenM1Pos=analogRead(potenMeter1);
  potenM2Pos=analogRead(potenMeter1);


}
