// v1.0
// Reciever
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------

#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

// led pinout
#define LED 12

// buzzer pinout
#define BUZZER1 18
#define BUZZER2 22

// servo pinout
#define GPIOTrottle 4
#define GPIOYaw 5
#define GPIOPitch 19
#define GPIORoll 21
#define GPIOMode 23

// -------------------- variables --------------------

// task
TaskHandle_t Task1;
TaskHandle_t Task2;

// count
int count=0;

// blinkcount
int blinkCount=0;

// com config
int com=1; // set 1 if ESP-NOw and 2 if SERVER (internet)

// com ESP-NOW
uint8_t myMac[]={0x40,0x22,0xD8,0x03,0x2E,0x50};
uint8_t targetMac[]={0x40,0x22,0xD8,0x08,0xBB,0x48};

// peerinfo
esp_now_peer_info_t peerInfo;

// wificonfig
const char* ssid="Onahs!-Hotspot-AP";
const char* pass="0x2m0q9G0z7VLIZjdHuCTMXwCU2NywNT";

// server connection 
HTTPClient http;

// server response
int server;

//time for ping
long int time1;
long int time2;
long int timePing;

// com server
String serverName="https://blynk.cloud/external/api/get?token=Z28VmfqlAHMfu1cQrnFKYZ5RFfK0lyXP&v0";

// servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// raw data
String rTrottle;
String rYaw;
String rPitch;
String rRoll;
String rMode;
String rCount;

// process data
int Trottle;
int Yaw;
int Pitch;
int Roll;
int Mode;
int rcvCount;
String Mods;

// percent data
int pTrottle;
int pYaw;
int pPitch;
int pRoll;

// counter incase of lost signal
int subCount;
int lastsubCount;
int lostCount=0;

// storage xMsg
// com 1
typedef struct struct_message{
  int trottle;
  int yaw;
  int pitch;
  int roll;
  int mode;
  int count;
}struct_message;
struct_message rcvxMsg;

// com 2
String xMsg;

// connection and send data espnow
String comStatus;
String msgStatus;

// -------------------- fuctions --------------------

// ---------- startup ----------

// version
void initVerInfo(){
  Serial.println("");
  Serial.println("v1.0");
  Serial.println("Reciever");
  Serial.println("@lolenseu");
  Serial.println("https://github.com/lolenseu");
  Serial.println("");
  delay(2000);
}

// initlogo
void initLogo() {
  Serial.println("");
  Serial.println("     ⠀⠀⠀⠀⠀⣀⣀⣀⣀⣠⣤⣤⣄⣀⣀⣀⣀⠀⠀⠀⠀⠀     ");
  Serial.println("     ⢀⣠⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣄⡀     ");
  Serial.println("     ⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷     ");
  Serial.println("     ⣿⣿⣿⡿⠛⠉⠉⠙⠿⣿⣿⣿⣿⠿⠋⠉⠉⠛⢿⣿⣿⣿     ");
  Serial.println("     ⣿⣿⣿⣶⣿⣿⣿⣦⠀⢘⣿⣿⡃⠀⣴⣿⣿⣿⣶⣿⣿⣿     ");
  Serial.println("     ⣿⣿⣿⣏⠉⠀⠈⣙⣿⣿⣿⣿⣿⣿⣋⠁⠀⠉⣹⣿⣿⣿     ");
  Serial.println("     ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿     ");
  Serial.println("     ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿     ");
  Serial.println("     ⢸⣿⣿⣎⠻⣿⣿⣿⣿⡿⠋⠙⢿⣿⣿⣿⣿⠟⣱⣿⣿⡇     ");
  Serial.println("     ⠀⢿⣿⣿⣧⠀⠉⠉⠉⠀⢀⡀⠀⠉⠉⠉⠀⣼⣿⣿⡿⠀     ");
  Serial.println("     ⠀⠈⢻⣿⣿⣷⣶⣶⣶⣶⣿⣿⣶⣶⣶⣶⣾⣿⣿⡟⠁⠀     ");
  Serial.println("     ⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⠉⠉⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀     ");
  Serial.println("     ⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⠀⠀⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀     ");
  Serial.println("     ⠀⠀⠀⠀⠀⠀⠀⠙⠻⢿⣦⣴⡿⠟⠋⠀⠀⠀⠀         ");
  Serial.println("");
  delay(3000);
}

// initBoot
void initBoot(){
  Serial.println("");
  Serial.println("Botting ...");
  Serial.println("");

  //Startup tone
  delay(3000);
  tone(BUZZER1,800,350);
  tone(BUZZER1,1000,350);
  tone(BUZZER1,1800,550);
  delay(1500);
}

// ---------- connection ----------

// initwifi
void initWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to WiFi ..");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
}

// initcom1
void initespnow(){
  WiFi.mode(WIFI_STA);
  Serial.println("Initiating ESP-NOW ..");

  // Init ESP-NOW
  if(esp_now_init()!=ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  delay(500);
}

// inticom2
void initserver(){

  // time for start ping
  time1=millis();

  // rcvmsg to server
  String serverPath=serverName;
  http.begin(serverPath);
  server=http.GET();
  if(server>0)xMsg=http.getString();

  // time for end ping
  time2=millis();

  // ping
  timePing=time2-time1;                                          
}

// ---------- processing ----------

// map to percent
int mapPercent(int toMapPercent){
  int mapValuePercent=map(toMapPercent,1000,2000,0,100);
  return mapValuePercent;
}

// mapmode
void mapMode(int toMode){
  int mapMode=map(toMode,1000,2000,1000,2000);
  if(mapMode>1000&&mapMode<1230)Mods="Stab";
  else if(mapMode>1231&&mapMode<1360)Mods="PosH";
  else if(mapMode>1361&&mapMode<1490)Mods="AltH";
  else if(mapMode>1391&&mapMode<1621)Mods="Loit";
  else if(mapMode>1621&&mapMode<1749)Mods="RTL ";
  else if(mapMode>1750&&mapMode<2000)Mods="Land";
}

// esp-now
void OnDataRecv(const uint8_t * mac,const uint8_t * incomingData,int len){
  memcpy(&rcvxMsg,incomingData,sizeof(rcvxMsg));

  // processed data
  Trottle=rcvxMsg.trottle;
  Yaw=rcvxMsg.yaw;
  Pitch=rcvxMsg.pitch;
  Roll=rcvxMsg.roll;
  Mode=rcvxMsg.mode;
  subCount=rcvxMsg.count;
}

// ---------- printing ----------

void serialDebug(){
  Serial.println("\n");
  Serial.println("-------------------- debug --------------------");
  if(com==1){
    Serial.println("ESP-NOw");
    Serial.printf("Com Status: ");
    Serial.println(comStatus);
    Serial.printf("Msg Status: ");
    Serial.println(msgStatus);
    Serial.println("");
  }
  if(com==2){
    Serial.println("WiFi");
    Serial.printf("RSSI: ");
    Serial.println(WiFi.RSSI());
    Serial.printf("Ping: ");
    Serial.println(timePing);
    Serial.println("");
  }
  Serial.println("Raw Data");
  Serial.printf("Trottle: %s\n",rTrottle);
  Serial.printf("Yaw: %s\n",rYaw);
  Serial.printf("Pitch: %s\n",rPitch);
  Serial.printf("Roll: %s\n",rRoll);
  Serial.printf("Mode: %s\n",rMode);
  Serial.println("");
  Serial.println("Processed Data");
  Serial.printf("Trottle: %d\n",Trottle);
  Serial.printf("Yaw: %d\n",Yaw);
  Serial.printf("Pitch: %d\n",Pitch);
  Serial.printf("Roll: %d\n",Roll);
  Serial.printf("Mode: %d\n",Mode);
  Serial.println("");
  Serial.println("Official Data");
  Serial.printf("Trottle: %d%%\n",pTrottle);
  Serial.printf("Yaw: %d%%\n",pYaw);
  Serial.printf("Pitch: %d%%\n",pPitch);
  Serial.printf("Roll: %d%%\n",pRoll);
  Serial.printf("Mode: %s\n",Mods);
  Serial.println("");
  Serial.println("Lost Counter");
  Serial.printf("SubCount: %d\n",subCount);
  Serial.printf("LostbCount: %d\n",lostCount);
  Serial.println("");
  Serial.println("Official Counter");
  Serial.printf("Count: %d\n",count);
  Serial.println("-------------------- debug --------------------");
}

// -------------------- task1 --------------------

void Task1code(void * pvParameters){
  for (;;) {
    // counter and buzzer
    if(count==100)count=0,tone(BUZZER1,3500,250);
    count+=1;

    // led blinker
    if(blinkCount==300)digitalWrite(LED, HIGH);
    if(blinkCount==305)digitalWrite(LED, LOW);
    if(blinkCount==310)digitalWrite(LED, HIGH);
    if(blinkCount==315)digitalWrite(LED, LOW),blinkCount=0;
    blinkCount+=1;

    // ---------- receive data ----------

    // rcv msg via ESP-NOW
    if(com==1){}

    // rcv msg via request
    if(com==2){

      // raw data
      rTrottle=xMsg.substring(2,6);
      rYaw=xMsg.substring(6,10);
      rPitch=xMsg.substring(10,14);
      rRoll=xMsg.substring(14,18);
      rMode=xMsg.substring(18,22);
      rCount=xMsg.substring(22,24);

      // processed data
      Trottle=rTrottle.toInt();
      Yaw=rYaw.toInt();
      Pitch=rPitch.toInt();
      Roll=rRoll.toInt();
      Mode=rMode.toInt();
      subCount=rCount.toInt();
    }

    // emergency servo protocol auto land
    if(subCount==lastsubCount){
      lostCount+=1; // lost counter
      if(lostCount>=100){

        // buzzer warning
        tone(BUZZER2,1000,150);
        tone(BUZZER2,800,350);

        // led warning
        if(count==10)digitalWrite(LED, HIGH);
        if(count==20)digitalWrite(LED, LOW);
        if(count==30)digitalWrite(LED, HIGH);
        if(count==40)digitalWrite(LED, LOW);
        if(count==50)digitalWrite(LED, HIGH);
        if(count==60)digitalWrite(LED, LOW);
        if(count==70)digitalWrite(LED, HIGH);
        if(count==80)digitalWrite(LED, LOW);
        if(count==90)digitalWrite(LED, HIGH);
        if(count==100)digitalWrite(LED, LOW);

        // stay on position
        Trottle=1580; // increase hight by 2%
        Yaw=1500;
        Pitch=1500;
        Roll=1500;
        Mode=1420; // 1420 loiter mode
      }
      if(lostCount>=500){
        lostCount=500;
        Mode=1690; // 1690 RTL mode
      }
    }
    else{
      lastsubCount=subCount;
      lostCount=0;
    }

    // write servo
    servo1.write(Trottle);
    servo2.write(Yaw);
    servo3.write(Pitch);
    servo4.write(Roll);
    servo5.write(Mode);

    // percent data
    pTrottle=mapPercent(Trottle);
    pYaw=mapPercent(Yaw);
    pPitch=mapPercent(Pitch);
    pRoll=mapPercent(Roll);
    mapMode(Mode);

    // ---------- debug data ----------

    // srial debug
    if(count==1||count==26||count==51||count==76)serialDebug(); // enable this for long debug
    //serialDebug(); // enable this for short debug if delay != 1000 = fast

    // delay
    delay(10); // run delay
    //delay(100); // test delay
    //delay(1000); // debug delay
    //delay(60000); // stop delay
  }
}

// -------------------- task2 --------------------

void Task2code(void * pvParameters){
  for(;;){

    // rcv msg via ESP-NOW
    if(com==1){}

    // rcv msg via request
    if(com==2)initserver();

    // disable delay on task 2 wen normal run
    //delay(1000); // debug delay
    //delay(60000); // stop delay
  } 
}

// -------------------- setup --------------------

void setup(){
  // put your setup code here, to run once:
  // Initialize Serial Monitor
  Serial.begin(115200);

  // startup
  initVerInfo(); // version
  initLogo(); // logo

  if(com==1){
    // intCom1 ESP-NOW
    initespnow();
  }
  if(com==2){
    // initWiFi
    initWiFi();

    // intCom2 Internet
    initserver();
  }

  // led
  pinMode(LED,OUTPUT);

  // buzzer
  pinMode(BUZZER1,OUTPUT);
  pinMode(BUZZER2,OUTPUT);

  // servo
  servo1.attach(GPIOTrottle);
  servo2.attach(GPIOYaw);
  servo3.attach(GPIOPitch);
  servo4.attach(GPIORoll);
  servo5.attach(GPIOMode);

  initBoot(); // boot

  // startup delay
  delay(300);

  // task handler
  xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,2,&Task1,0);
  xTaskCreatePinnedToCore(Task2code,"Task2",10000,NULL,1,&Task2,1);
}

// -------------------- loop --------------------
 
void loop(){
  // put your main code here, to run repeatedly:
  
}
