#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//Pinouts
RF24 radio(1, 3);

//Joystic Pinouts
int joyX1 = 15;
int joyY1 = 2;
int joySW1 = 17;

int joyX2 = 0;
int joyY2 = 4;
int joySW2 = 16;

//Potentiometer Pinouts
int potenMeter1 = 12;
int potenMeter2 = 14;


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

const byte address[6] = "00001";


void setup() {
  Serial.begin(115200);
  radio.setChannel(2);
  radio.openWritingPipe(address);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  pinMode(joyX1, INPUT);
  pinMode(joyY1, INPUT);
  pinMode(joySW1, INPUT_PULLUP);

  pinMode(joyX2, INPUT);
  pinMode(joyY2, INPUT);
  pinMode(joySW2, INPUT_PULLUP);

  pinMode(potenMeter1, INPUT);
  pinMode(potenMeter2, INPUT);  
  
}


void loop() {
  joyX1Pos = analogRead(joyX1);
  joyY1Pos = analogRead(joyY1);
  joySW1State = digitalRead(joySW1);

  joy_mapX1 = map(joyX1Pos, 0, 1023, 1000, 2000);
  joy_mapY1 = map(joyY1Pos, 0, 1023, 1000, 2000);

  joyX2Pos = analogRead(joyX2);
  joyY2Pos = analogRead(joyY2);
  joySW2State = digitalRead(joySW2);

  joy_mapX2 = map(joyX2Pos, 0, 1023, 1000, 2000);
  joy_mapY2 = map(joyY2Pos, 0, 1023, 1000, 2000);

  potenM1Pos = analogRead(potenMeter1);
  potenM2Pos = analogRead(potenMeter1);


  
  radio.write(&msg, sizeof(msg));
}
