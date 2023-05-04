// v1.0
// Reciever
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------

#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

// buzzer pinout
#define BUZZER 18

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

// com config
int com=2; // set 1 if ESP-NOw and 2 if SERVER (internet)

// wificonfig
const char* ssid="lolenseu";
const char* pass="@lolenseu24!";

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

// storage message
typedef struct structMsg{
    int trottle;
    int yaw;
    int pitch;
    int roll;
    int mode;
}
structMsg;
structMsg rcvMsg;

// starage xmessage
String xMsg;

// variable to store if receiving data was successful or bad
String msgStatus1;
String msgStatus2;

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

  //DJI tone start up
  delay(3000);
  tone(BUZZER,800,550);
  delay(300);
  tone(BUZZER,1200,550);
  delay(200);
  tone(BUZZER,1050,550);
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
void initCom1(){
  WiFi.mode(WIFI_STA);
  Serial.print("Initiating ESP-NOW ..");

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
void initCom2(){

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
  int mapMode=map(toMode,1000,2000,1,6);
  if(mapMode==1)Mods="Stab";
  else if(mapMode==2)Mods="At.H";
  else if(mapMode==3)Mods="Loit";
  else if(mapMode==4)Mods="Guid";
  else if(mapMode==5)Mods="ReHo";
  else if(mapMode==6)Mods="Land";
}

// esp-now
void OnDataRecv(const uint8_t * mac,const uint8_t * incomingData,int len){
  memcpy(&rcvMsg,incomingData,sizeof(rcvMsg));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(rcvMsg.trottle);
  Serial.println(rcvMsg.yaw);
  Serial.println(rcvMsg.pitch);
  Serial.println(rcvMsg.roll);
  Serial.println();
}

// ---------- printing ----------

void serialDebug(){
  Serial.println("\n");
  Serial.println("-------------------- debug --------------------");
  if(com==1){
    Serial.println("ESP-NOw");
    Serial.printf("Snd Status: ");
    Serial.println(msgStatus1);
    Serial.printf("Msg Status: ");
    Serial.println(msgStatus2);
    Serial.println("\n");
  }
  if(com==2){
    Serial.println("WiFi");
    Serial.printf("RSSI: ");
    Serial.println(WiFi.RSSI());
    Serial.printf("Ping: ");
    Serial.println(timePing);
    Serial.println("\n");
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
  Serial.printf("LastsubCount: %d\n",lastsubCount);
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
    if(count==100)count=0,tone(BUZZER,3500,250);
    count+=1;

    // ---------- prepare data ----------

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

    // percent data
    pTrottle=mapPercent(Trottle);
    pYaw=mapPercent(Yaw);
    pPitch=mapPercent(Pitch);
    pRoll=mapPercent(Roll);
    mapMode(Mode); 

    // write data to servo
    if(WiFi.status()!=WL_CONNECTED||server!=200||subCount==lastsubCount){
      lostCount+=1; // lost counter
      if(lostCount>=3000){
        lostCount=3000;

        // emergency servo protocol auto land
        servo1.write(1500);
        servo2.write(1500);
        servo3.write(1500);
        servo4.write(1500);
        servo5.write(1899); // 1899 land mode in ardupilot
      }
    }
    else{
      lastsubCount=subCount;
      lostCount=0;

      // write servo
      servo1.write(Trottle);
      servo2.write(Yaw);
      servo3.write(Pitch);
      servo4.write(Roll);
      servo5.write(Mode);
    }

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

    // ---------- send data ----------

    // rcv msg via ESP-NOW
    if(com==1){
      /*
      esp_err_t result;
    
      if (msgMode == 1) result = esp_now_send(targetMac, (uint8_t *) &sndMsg, sizeof(sndMsg));
      if (msgMode == 2) result = esp_now_send(targetMac, (uint8_t *) &xMsg, sizeof(xMsg));
      
      if (result == ESP_OK) msgStatus1 = "ok!";
      else msgStatus1 = "bd!";
      */
    }

    // rcv msg via request
    if(com==2){
      initCom2();
    }
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
    initCom1();
  }
  if(com==2){
    // initWiFi
    initWiFi();

    // intCom2 Internet
    initCom2();
  }

  // buzzer
  pinMode(BUZZER,OUTPUT);

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
