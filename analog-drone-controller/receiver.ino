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

// serial pinout
#define RXD 16
#define TXD 17

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

// ---------- manualvar ----------

// com config
int com=1; // set 1 if ESP-NOw and 2 if SERVER (internet)

// com ESP-NOW
uint8_t myMac[]={0x40,0x22,0xD8,0x03,0x2E,0x50};
uint8_t targetMac[]={0x40,0x22,0xD8,0x08,0xBB,0x48};

// wificonfig
const char* ssid="Onahs!-Hotspot-AP";
const char* pass="0x2m0q9G0z7VLIZjdHuCTMXwCU2NywNT";

// com server
String serverUrl="https://blynk.cloud/external/api/get?token=Z28VmfqlAHMfu1cQrnFKYZ5RFfK0lyXP&v0";

// ---------- fixvar ----------

// task
TaskHandle_t cpu1;
TaskHandle_t cpu2;

// counter
int loop1=0;
int loop2=0;

// blinkcount
int blinkCount=0;

// peerinfo
esp_now_peer_info_t peerInfo;

// server connection 
HTTPClient http;

// server response
int server;

//time for ping
long int time1;
long int time2;
long int timePing;

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

// counter incase of lost signal
int subCount;
int lastsubCount;
int lostCount=0;

// percent data
int pTrottle;
int pYaw;
int pPitch;
int pRoll;

// connection and send data espnow
String comStatus;
String msgStatus;

// storage xMsg
// com 1
typedef struct struct_message_snd{
  char data[128];
}struct_message_snd;
struct_message_snd sndxMsg;

typedef struct struct_message_rcv{
  int trottle;
  int yaw;
  int pitch;
  int roll;
  int mode;
  int loop1;
  char data[128];
}struct_message_rcv;
struct_message_rcv rcvxMsg;

// com 2
String xMsg;

// -------------------- fuctions --------------------

// ---------- startup ----------

// initBoot
void initBoot(){
  Serial.println("");
  Serial.println("Botting ...");
  Serial.println("");

  //Startup tone
  tone(BUZZER1,2000,500);
  tone(BUZZER1,3000,300);
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
    Serial.println("Error Initializing ESP-NOW");
    return;
  }

  // Register peer
  memcpy(peerInfo.peer_addr,targetMac,6);
  peerInfo.channel=0;  
  peerInfo.encrypt=false;
  if(esp_now_add_peer(&peerInfo)!=ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));
  delay(500);
}

// inticom2
void initserver(){

  // time for start ping
  time1=millis();

  // rcvmsg to server
  String serverPath=serverUrl;
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
  else if(mapMode>1491&&mapMode<1621)Mods="Loit";
  else if(mapMode>1621&&mapMode<1749)Mods="RTL ";
  else if(mapMode>1750&&mapMode<2000)Mods="Land";
}

// esp-now
void OnDataSent(const uint8_t * mac_addr,esp_now_send_status_t status){
  if(status==ESP_NOW_SEND_SUCCESS)comStatus="ok!";
  else comStatus="bd!";
}

void OnDataRecv(const uint8_t *mac_addr,const uint8_t *incomingData,int data_len){
  memcpy(&rcvxMsg,incomingData,sizeof(rcvxMsg));
}

// ---------- printing ----------

void serialDebug(){
  Serial.println("\n");
  Serial.println("-------------------- debug --------------------");
  if(com==1){
    Serial.println("ESP-NOW");
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
  Serial.printf("Cpu1: %d\n",loop1);
  Serial.printf("Cpu2: %d\n",loop2);
  Serial.println("-------------------- debug --------------------");
}

// -------------------- task1 --------------------

void Task1code(void * pvParameters){
  for (;;) {
    // cpu1 counter and buzzer
    loop1+=1;
    if(loop1==100)loop1=0,tone(BUZZER1,3500,250);

    // led blinker
    if(blinkCount==5)digitalWrite(LED, HIGH);
    if(blinkCount==10)digitalWrite(LED, LOW);
    if(blinkCount==15)digitalWrite(LED, HIGH);
    if(blinkCount==20)digitalWrite(LED, LOW);
    if(blinkCount==200)blinkCount=0;
    blinkCount+=1;

    // ---------- receive data ----------

    // msg via ESP-NOW
    if(com==1){
      // processed data
      Trottle=rcvxMsg.trottle;
      Yaw=rcvxMsg.yaw;
      Pitch=rcvxMsg.pitch;
      Roll=rcvxMsg.roll;
      Mode=rcvxMsg.mode;
      subCount=rcvxMsg.loop1;

      // serial uart
      while(Serial.available()>=sizeof(sndxMsg.data)){
        Serial.readBytes(sndxMsg.data,sizeof(sndxMsg.data));
      }
      if(Serial.availableForWrite()>=sizeof(rcvxMsg.data)){
        Serial.write(rcvxMsg.data,sizeof(rcvxMsg.data));
      }
    }

    // msg via request
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
    
    // ---------- prepare data ----------

    // emergency servo protocol auto land
    if(subCount==lastsubCount){
      lostCount+=1; // lost counter
      if(lostCount>=100){
        if(lostCount>=100&&lostCount<=1900){
          if(loop1==1)tone(BUZZER2,1000,200);
        }

        // stay on position
        Trottle=1720; // increase hight by 20%
        Yaw=1500;
        Pitch=1500;
        Roll=1500;
        Mode=1540; // Loiter mode
      }
      if(lostCount>=2000){

        // buzzer warning for return to land
        if(loop1==0||loop1==25||loop1==50||loop1==75)tone(BUZZER2,1000,200);

        // led warning
        if(loop1==76)digitalWrite(LED, HIGH);

        // Return to Land
        Mode=1690; // RTL mode
      }
      if(lostCount>=10000){
        lostCount=10000;

        // buzzer warning for search if lost
        if(loop1==10||loop1==35||loop1==60||loop1==85)tone(BUZZER2,200,200);
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

    // serial debug
    if(loop1==0||loop1==20||loop1==40||loop1==60||loop1==80)serialDebug(); // enable this for long debug
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
    // cpu2 counter
    loop2+=1;
    if(loop2==100)loop2=0;

    // ---------- send data ----------

    // msg via ESP-NOW
    if(com==1){
      esp_err_t result;
      result=esp_now_send(targetMac,(uint8_t *)&sndxMsg,sizeof(sndxMsg)); 
      if(result==ESP_OK)msgStatus="1";
      else msgStatus="0";
    }

    // msg via request
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
  Serial2.begin(9600,SERIAL_8N1, RXD, TXD);

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
  xTaskCreatePinnedToCore(Task1code,"cpu1",10000,NULL,2,&cpu1,0);
  xTaskCreatePinnedToCore(Task2code,"cpu2",10000,NULL,1,&cpu2,1);
}

// -------------------- loop --------------------
 
void loop(){
  // put your main code here, to run repeatedly:
  
}
