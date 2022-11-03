#define BLYNK_TEMPLATE_ID           "TMPLUTIHhFl2"
#define BLYNK_DEVICE_NAME           "Drone Controller"
#define BLYNK_AUTH_TOKEN            "hQZ_-DZa6AlE4034MKhE_VwDb2qlrD6A"
#define WIFI_SSID                   "apm2.8-hexacopter"
#define WIFI_PASSWORD               "1234567890"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoTimer.h>
#include <Servo.h>


ArduinoTimer Timer1;
ArduinoTimer Timer2;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

int counter = 0;

int BUZZER = 5;
int LED1 = 16;

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = WIFI_SSID;                    
char pass[] = WIFI_PASSWORD;

int rcValue1;
int rcValue2;
int rcValue3;
int rcValue4;
int rcValue5;
int rcValue6;
int rcValue7;
int rcValue8;
int rcValue9;
int rcValue10;

int prcValue1;
int prcValue2;
int prcValue3;
int prcValue4;
int prcValue5;

int nrcValue1;
int nrcValue2;
int nrcValue3;
int nrcValue4;
int nrcValue5;


BLYNK_WRITE(V1) {
  rcValue1 = param.asInt();
  prcValue1 = map(rcValue1,1000,2000,1000,2000);
  if (rcValue9 == 1) nrcValue1 = map(prcValue1,1000,2000,1400,1800);
  else if (rcValue1 == 1000) tone(BUZZER,5000);
  else nrcValue1 = prcValue1;
}

BLYNK_WRITE(V2) {
  rcValue2 = param.asInt();
  prcValue2 = map(rcValue2,1000,2000,2000,1000);
  if (rcValue9 == 1) nrcValue2 = map(prcValue2,2000,1000,1700,1300);
  else nrcValue2 = prcValue2;
}

BLYNK_WRITE(V3) {
  rcValue3 = param.asInt();
  prcValue3 = map(rcValue3,1000,2000,2000,1000);
  if (rcValue10 == 1) nrcValue3 = map(prcValue3,2000,1000,1700,1300);
  else nrcValue3 = prcValue3;
}

BLYNK_WRITE(V4) {
  rcValue4 = param.asInt();
  prcValue4 = map(rcValue4,1000,2000,2000,1000);
  if (rcValue10 == 1) nrcValue4 = map(prcValue4,2000,1000,1700,1300);
  else nrcValue4 = prcValue4;
}

BLYNK_WRITE(V5) {
  rcValue5 = param.asInt();
  nrcValue5 = map(rcValue5,1,6,1100,1900);
}

BLYNK_WRITE(V6) {
  rcValue6 = param.asInt();
}

BLYNK_WRITE(V7) {
  rcValue7 = param.asInt();
  if (rcValue7 == 1) tone(BUZZER,5000);
  else noTone(BUZZER);
}

BLYNK_WRITE(V8) {
  rcValue8 = param.asInt();
  if (rcValue8 == 1) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}

BLYNK_WRITE(V9) {
  rcValue9 = param.asInt();
}

BLYNK_WRITE(V10) {
  rcValue10 = param.asInt();
}


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  servo1.attach(12);
  servo2.attach(14);
  servo3.attach(13);
  servo4.attach(15);
  servo5.attach(2);
  servo6.attach(0);
  
  //DJI Tone Start Up
  delay(8000);
  tone(BUZZER,800,550);
  delay(300);
  tone(BUZZER,1200,550);
  delay(200);
  tone(BUZZER,1050,550);
  delay(1000);
  
  //5sec. Delay.
  delay(5000);
}


void loop() {
  if (Blynk.connected()) {
    Blynk.run();

    //Buzzer and Led.
    if (Timer1.TimePassed_Milliseconds(3000)) {
      digitalWrite(LED1, HIGH);
      tone(BUZZER,3500,250);
    }
    else if (Timer2.TimePassed_Milliseconds(3500)) {
      digitalWrite(LED1, LOW);
      Timer1.Reset();
      Timer2.Reset();
    }

    //Saver Functions.
    //Auto Loiter if Untouch.
    if (counter == 250) {
      if (nrcValue5 == 1580) nrcValue5 = 1580, counter = 0;
      else if (nrcValue5 == 1740) nrcValue5 = 1740, counter = 0;
      else if (nrcValue5 == 1900) nrcValue5 = 1900, counter = 0;
      else if (rcValue3 == 1500 and rcValue4 == 1500) nrcValue5 = 1420, counter = 300;
      else counter = 0;
    }
    
    //Auto Landing if Loiter Mode Untouch.
    else if (counter == 1000) {
      if (nrcValue5 == 1420) nrcValue5 = 1899, counter = 0;
      else counter = 0;
    }
    
    counter += 1;
    
    //Sending PWM Signal to ArduPilot.
    servo1.write(nrcValue1);
    servo2.write(nrcValue2);
    servo3.write(nrcValue3);
    servo4.write(nrcValue4);
    servo5.write(nrcValue5);
  
    if (rcValue6 == 1) servo6.write(nrcValue1);
    else servo6.write(2000);
    
    Blynk.virtualWrite(V12, map(nrcValue5,1100,1900,1,6));

    //Debugging.
    Serial.print("Trottle: ");
    Serial.print(rcValue1);
    Serial.print(" / ");
    Serial.print(nrcValue1);
    Serial.print("\t");
    Serial.print("Yaw: ");
    Serial.print(rcValue2);
    Serial.print(" / ");
    Serial.print(nrcValue2);
    Serial.print("\t");
    Serial.print("Pitch: ");
    Serial.print(rcValue3);
    Serial.print(" / ");
    Serial.print(nrcValue3);
    Serial.print("\t");
    Serial.print("Roll: ");
    Serial.print(rcValue4);
    Serial.print(" / ");
    Serial.print(nrcValue4);
    Serial.print("\t");
    Serial.print("Mode: ");
    Serial.print(rcValue5);
    Serial.print(" / ");
    Serial.print(nrcValue5);
    Serial.print("\t");
    Serial.print("Counter: ");
    Serial.println(counter);
    //delay(100); //Enable this for Debugging.
  }
  //Auto Landing if Disconnected to the Internet.
  else {
    servo1.write(1600);
    servo2.write(1500);
    servo3.write(1500);
    servo4.write(1500);
    servo5.write(1899);
    digitalWrite(LED1, LOW);
    tone(BUZZER,5000,250);
    delay(2500);
    digitalWrite(LED1, HIGH);
    Blynk.run();
  }
}
