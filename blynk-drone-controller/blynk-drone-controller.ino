#define BLYNK_TEMPLATE_ID           "TMPLUTIHhFl2"
#define BLYNK_DEVICE_NAME           "Drone Controller"
#define BLYNK_AUTH_TOKEN            "hQZ_-DZa6AlE4034MKhE_VwDb2qlrD6A"
#define WIFI_SSID                   "apm2.8-hexacopter"
#define WIFI_PASSWORD               "1234567890"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = WIFI_SSID;                    
char pass[] = WIFI_PASSWORD;

Servo servo1,servo2,servo3,servo4,servo5,servo6;

int counter = 0;

int BUZZER = 5;
int LED1 = 16;

int prcValue1;

int rcValue1 = 1600;
int rcValue2 = 1500;
int rcValue3 = 1500;
int rcValue4 = 1500;
int rcValue5 = 1100;
int rcValue6 = 2000;
int rcValue7 = 0;
int rcValue8 = 0;
int rcValue9 = 0;
int rcValue10 = 0;


BLYNK_WRITE(V1) {
  if (rcValue9 == 1) {
      prcValue1 = param.asInt();
    //prcValue1 = map(param.asInt(),1000,2000,1400,1800);
  }
  else rcValue1 = param.asInt();
  //servo1.write(rcValue1);
}

BLYNK_WRITE(V2) {
  if (param.asInt()) {
    if (rcValue9 == 1) rcValue2 = map(param.asInt(),1000,2000,1700,1300);
    else rcValue2 = map(param.asInt(),1000,2000,2000,1000);
    //servo2.write(rcValue2);
  }
  else rcValue2 = 1500;
}

BLYNK_WRITE(V3) {
  if (param.asInt()) {
    if (rcValue10 == 1) rcValue3 = map(param.asInt(),1000,2000,1700,1300);
    else rcValue3 = map(param.asInt(),1000,2000,2000,1000);
    //servo3.write(rcValue3);
  }
  else rcValue3 = 1500;
}

BLYNK_WRITE(V4) {
  if (param.asInt()) {
    if (rcValue9 == 1) rcValue4 = map(param.asInt(),1000,2000,1700,1300);
    else rcValue4 = map(param.asInt(),1000,2000,2000,1000);
    //servo4.write(rcValue4);
  }
  else rcValue4 = 1500;
}

BLYNK_WRITE(V5) {
  rcValue5 = map(param.asInt(),1,6,1100,1900);
 // servo5.write(rcValue5);
}

BLYNK_WRITE(V6) {
  rcValue6 = param.asInt();
  //if (rcValue6 == 1) servo6.write(rcValue1);
  //else servo6.write(2000);
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
  delay(3000);
  tone(BUZZER,800,550);
  delay(300);
  tone(BUZZER,1200,550);
  delay(200);
  tone(BUZZER,1050,550);
  delay(1500);
  
  //5sec. Delay.
  delay(5000);
}


void loop() {
  if (Blynk.connected()) {
    
    //Run the Blynk Fuctions.
    Blynk.run();

    //Tuning Throttle.
    if (rcValue9 == 1) {
      if (prcValue1 > 1600) rcValue1 += 2;
      if (prcValue1 < 1400) rcValue1 -= 2;
      
      if (rcValue1 >= 1800) prcValue1 = 1600,rcValue1 = 1600;
      if (rcValue1 <= 1400) prcValue1 = 1600,rcValue1 = 1600;
    }

    //Sending Signal to ArduPilot.
    servo1.write(rcValue1);
    servo2.write(rcValue2);
    servo3.write(rcValue3);
    servo4.write(rcValue4);
    servo5.write(rcValue5);

    //For Manual ESC Calibration
    if (rcValue6 == 1) servo6.write(rcValue1);
    else servo6.write(2000);
    
    //Led, Buzzer, and Ping.
    if (counter == 1) {
      digitalWrite(LED1, HIGH);
      tone(BUZZER,3500,250);
    }

    if (counter == 10) {
      digitalWrite(LED1, LOW);     
    }
    
    if (counter == 100) {
      Ping.ping("sgp1.blynk.cloud", 1); //Edit this to your Blynk Server.
      Blynk.virtualWrite(V11, Ping.averageTime());
      Blynk.virtualWrite(V12, map(rcValue5,1100,1900,1,6));
      counter = 0;
    }
    
    counter += 1;

    //Debugging.
    Serial.printf("Trottle: %d  |  Yaw: %d  |  Pitch: %d  |  Roll: %d  |  Mode: %d  |  Counter: %d \n",rcValue1,rcValue2,rcValue3,rcValue4,rcValue5,counter);
    //delay(100); //Enable this for Debugging.
  }
   
  //Auto Landing if Disconnected to the Internet.
  else {
    servo1.write(1600);
    servo2.write(1500);
    servo3.write(1500);
    servo4.write(1500);
    servo5.write(1899); //ArduPilot Mode No.6 "Land".
    digitalWrite(LED1, HIGH);
    tone(BUZZER,5000,250);
    delay(500);
    digitalWrite(LED1, LOW);
    delay(1000);
    Blynk.run();
  }
}
