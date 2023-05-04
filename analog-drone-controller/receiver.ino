// v1.0
// Reciever
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------

#include <WiFi.h>
#include <ESP32Servo.h>
#include <esp_now.h>
#include <HTTPClient.h>

// buzzer
#define BUZZER 12

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

//time for ping
long int time1;
long int time2;
long int timePing;

// com server
String serverName="https://blynk.cloud/external/api/get?token=Z28VmfqlAHMfu1cQrnFKYZ5RFfK0lyXP&v0";

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

// -------------------- fuctions --------------------

// ---------- startup ----------

// version
void initVerInfo(){
  Serial.println("");
  Serial.println("v1.0");
  Serial.println("Reciever");
  Serial.println("@lolenseu");
  Serial.println("https://github.com/lolenseu");
  delay(2000);
}

// initlogo
void initLogo() {
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
  Serial.println("Botting ...");

  //DJI Tone Start Up
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

  // sendmsg and rcvmsg to server
  String serverPath=serverName;
  http.begin(serverPath);     
  if(http.GET()>0)xMsg=http.getString();

  // time for end ping
  time2=millis();

  // ping
  timePing=time2-time1;                                          
}

// ---------- processing ----------




// esp-now
void OnDataRecv(const uint8_t * mac,const uint8_t *incomingData,int len){
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

// -------------------- task1 --------------------

void Task1code(void * pvParameters){
  for (;;) {
    // counter and buzzer
    if(count==100)count=0,tone(BUZZER,3500,250);
    count+=1;
  }
}

// -------------------- task2 --------------------

void Task2code(void * pvParameters){
  for(;;){
    // ---------- send data ----------

    // send msg via ESP-NOW
    /*
    if (com == 1) {
      esp_err_t result;

      if (msgMode == 1) result = esp_now_send(targetMac, (uint8_t *) &sndMsg, sizeof(sndMsg));
      if (msgMode == 2) result = esp_now_send(targetMac, (uint8_t *) &xMsg, sizeof(xMsg));
      
      if (result == ESP_OK) msgStatus1 = "ok!";
      else msgStatus1 = "bd!";
    }
    */

    // send msg via request
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

  // buzzer pinout
  pinMode(BUZZER,OUTPUT);

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
