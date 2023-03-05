#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino resetpin)

//Joystic Pinouts
int joyX1=32;
int joyY1=33;
int joySW1=36;

int joyX2=25;
int joyY2=26;
int joySW2=39;

//Potentiometer Pinouts
int potenMeter1=14;
int potenMeter2=12;

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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  
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
  
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);
  display.println("Hello world!");
  display.display();
  display.clearDisplay();

}
