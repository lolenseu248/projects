#define BLYNK_TEMPLATE_ID           "TMPLUTIHhFl2"
#define BLYNK_DEVICE_NAME           "Drone Controller"
#define BLYNK_AUTH_TOKEN            "hQZ_-DZa6AlE4034MKhE_VwDb2qlrD6A"
#define WIFI_SSID                   "apm2.8-hexacopter"
#define WIFI_PASSWORD               "1234567890"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define BUZZER 5
#define LED1 16

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = WIFI_SSID;                    
char pass[] = WIFI_PASSWORD;                        

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  servo1.attach(12);
  servo2.attach(14);
  servo3.attach(13);
  servo4.attach(15);
  servo5.attach(2);
  servo6.attach(0);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
}

void loop() {
  if (Blynk.connected()) {
    Blynk.run();
  }
  //Auto Landing if Disconnected to the Internet
  else {
    servo1.write(1500);
    servo2.write(1500);
    servo3.write(1500);
    servo4.write(1500);
    servo5.write(1899);
    tone(BUZZER,5000,250);
    delay(1000);
    Blynk.run();
  }
}

BLYNK_WRITE(V1) {
  if (LED1 == 1) {
    int servov1 = map(param.asInt(),1000,2000,1300,1700);
    servo1.write(servov1);
  }
  else {
    int servov1 = param.asInt();
    servo1.write(servov1);
    if (servov1 == 1000) tone(BUZZER,5000);
    else noTone(BUZZER);
  }
}

BLYNK_WRITE(V2) {
  if (LED1 == 1) {
    int servov2 = map(param.asInt(),1000,2000,1700,1300);
    servo2.write(servov2);
  }
  else {
    int servov2 = map(param.asInt(),1000,2000,2000,1000);
    servo2.write(servov2);
  }
}

BLYNK_WRITE(V3) {
  if (LED1 == 1) {
    int servov3 = map(param.asInt(),1000,2000,1700,1300);
    servo3.write(servov3);
  }
  else {
    int servov3 = map(param.asInt(),1000,2000,2000,1000);
    servo3.write(servov3);
  } 
}

BLYNK_WRITE(V4) {
  if (LED1 == 1) {
    int servov4 = map(param.asInt(),1000,2000,1700,1300);
    servo4.write(servov4);
  }
  else {
     int servov4 = map(param.asInt(),1000,2000,2000,1000);
   servo4.write(servov4);
  }
}

BLYNK_WRITE(V5) {
  int servov5 = map(param.asInt(),1,6,1100,1900);
  servo5.write(servov5);
}
/*
BLYNK_WRITE(V6) {
  int servov6 = param.asInt();
  if (servov6 == 1) servo6.write(servov1);
  else servo6.write(2000);
}
*/
BLYNK_WRITE(V7) {
  int buzzerv7 = param.asInt();
  if (buzzerv7 == 1) tone(BUZZER,5000);
  else noTone(BUZZER);
}

BLYNK_WRITE(V8) {
  int ledv8 = param.asInt();
  if (ledv8 == 1) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}

BLYNK_WRITE(V9) {
  int buttonv9 = param.asInt();
}

BLYNK_WRITE(V10) {
  int buttonv10 = param.asInt();
}
