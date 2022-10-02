#define BLYNK_TEMPLATE_ID           "TMPLUTIHhFl2"
#define BLYNK_DEVICE_NAME           "Drone Controller"
#define BLYNK_AUTH_TOKEN            "hQZ_-DZa6AlE4034MKhE_VwDb2qlrD6A"
#define WIFI_SSID                   "apm2.8-hexacopter"
#define WIFI_PASSWORD               "1234567890"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define LED1 5
#define LED2 16

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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  //Auto Landing if Disconnected to the Internet
  if (Blynk.connected()) Blynk.run();
  else {
    servo1.write(1500);
    servo2.write(1500);
    servo3.write(1500);
    servo4.write(1500);
    servo5.write(1899);
    delay(1000);
    Blynk.run();
  }
}

BLYNK_WRITE(V1) {
  int servov1 = param.asInt();
  servo1.write(servov1);
  //Serial.println(servov1);
}

BLYNK_WRITE(V2) {
  int servov2 = map(param.asInt(),1000,2000,2000,1000);
  servo2.write(servov2);
  //Serial.println(servov2);
}

BLYNK_WRITE(V3) {
  int servov3 = map(param.asInt(),1000,2000,2000,1000);
  servo3.write(servov3);
  //Serial.println(servov3);
}

BLYNK_WRITE(V4) {
  int servov4 = map(param.asInt(),1000,2000,2000,1000);
  servo4.write(servov4);
  //Serial.println(servov4);
}

BLYNK_WRITE(V5) {
  int servov5 = map(param.asInt(),1,6,1100,1900);
  servo5.write(servov5);
  //Serial.println(servov5);
}

BLYNK_WRITE(V6) {
  int servov6 = param.asInt();
  servo6.write(servov6);
 //Serial.println(servov6);
}

BLYNK_WRITE(V7) {
  int ledv1 = param.asInt();
  if (ledv1 == 1) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
  //Serial.prinrln(ledv1);
}

BLYNK_WRITE(V8) {
  int ledv2 = param.asInt();
  if (ledv2 == 1) digitalWrite(LED2, HIGH);
  else digitalWrite(LED2, LOW);
  //Serial.println(ledv2);
}
