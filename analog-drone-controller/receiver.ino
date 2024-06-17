// Reciever
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------

#include <WiFi.h>
#include <MAVLink.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// serial pinout
#define RXD 16
#define TXD 17

// buzzer pinout
#define BUZZER 22

// servo pinout
#define GPIOTrottle 4
#define GPIOYaw 5
#define GPIOPitch 19
#define GPIORoll 21
#define GPIOMode 23

// -------------------- variables --------------------
// manualvar ----------
// esp-now mymac and targetmac
//uint8_t myMac[]={0x40,0x22,0xD8,0x03,0x2E,0x50};
uint8_t myMac[]={0x40,0x22,0xD8,0x05,0x68,0xB0};
uint8_t targetMac[]={0x40,0x22,0xD8,0x08,0xBB,0x48};

// fixvar ----------
// peerinfo
esp_now_peer_info_t peerInfo;

// task
TaskHandle_t cpu1;
TaskHandle_t cpu2;

// mavlink
mavlink_message_t msg;
mavlink_status_t status;

// mavlink heartbeattime
unsigned long lastHeartbeatTime=0;

// counter
int loop1=0;
int loop2=0;

// time
unsigned long globaltime;
unsigned long startTime1;
unsigned long startTime2;
unsigned long elapsedTime1;
unsigned long elapsedTime2;

// clock
unsigned long clock1=0;
unsigned long clock2=0;

// process data
int Trottle;
int Yaw;
int Pitch;
int Roll;
int Mode;
String Mods;

// servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// counter incase of lost signal
unsigned long losscount1=0;
unsigned long losscount2=0;
unsigned long losscount3=0;

// percent data
int percentTrottle;
int percentYaw;
int percentPitch;
int percentRoll;

// connection and send data espnow
String comStatus;
int ping;

// send_message
typedef struct send_message{
  int time1;
  int time2;
  uint16_t len;
  uint8_t buf[128];
};
send_message sndxMsg;

// recive_message
typedef struct receive_message{
  int trottle;
  int yaw;
  int pitch;
  int roll;
  int mode;
  int time1;
  int time2;
  uint16_t len;
  uint8_t buf[128];
};
receive_message rcvxMsg;

// -------------------- fuctions --------------------
// startup ----------
// initBoot
void initBoot(){
  Serial.println("");
  Serial.println("Botting ...");
  Serial.println("");

  //Startup tone
  digitalWrite(BUZZER,HIGH);
  delay(50);
  digitalWrite(BUZZER,LOW);
  delay(300);
}

// connection ----------
// init esp-now
void initespnow(){
  WiFi.mode(WIFI_STA);
  Serial.println("Initiating ESP-NOW ..");

  // init ESP-NOW
  if(esp_now_init()!=ESP_OK){
    Serial.println("Error Initializing ESP-NOW");
    return;
  }

  // register peer
  memcpy(peerInfo.peer_addr,targetMac,6);
  peerInfo.channel=0;  
  peerInfo.encrypt=false;
  if(esp_now_add_peer(&peerInfo)!=ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));
  delay(500);
}

// serial uart ----------
void serialuart(){
  // serial uart receive and write
  if(Serial2.availableForWrite()>0){
    Serial2.write(rcvxMsg.buf,rcvxMsg.len);
  }

  while(Serial2.available()>0){
    uint8_t c=Serial2.read();
    if (mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
      sndxMsg.len=mavlink_msg_to_send_buffer(sndxMsg.buf,&msg);
    }
  }
}

// processing ----------
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

// esp-now ----------
void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status){
  if(status==ESP_NOW_SEND_SUCCESS)comStatus="ok!";
  else comStatus="bd!";
}

void OnDataRecv(const uint8_t *mac_addr,const uint8_t *incomingData,int data_len){
  memcpy(&rcvxMsg,incomingData,sizeof(rcvxMsg));
}

// printing ----------
// serial debug
void serialDebug(){
  Serial.println("\n");
  Serial.println("-------------------- debug --------------------");
  Serial.println("ESP-NOW");
  Serial.printf("Com Status: ");
  Serial.println(comStatus);
  Serial.printf("ping: %dms\n",ping);
  Serial.println("");
  /*
  Serial.println("Processed Data");
  Serial.printf("Trottle: %d\n",Trottle);
  Serial.printf("Yaw: %d\n",Yaw);
  Serial.printf("Pitch: %d\n",Pitch);
  Serial.printf("Roll: %d\n",Roll);
  Serial.printf("Mode: %d\n",Mode);
  Serial.println("");
  */
  Serial.println("Official Data");
  Serial.printf("Trottle: %d%%\n",percentTrottle);
  Serial.printf("Yaw: %d%%\n",percentYaw);
  Serial.printf("Pitch: %d%%\n",percentPitch);
  Serial.printf("Roll: %d%%\n",percentRoll);
  Serial.printf("Mode: %s\n",Mods);
  Serial.println("");
  /*
  Serial.println("Lost Counter");
  Serial.printf("SubCount: %d\n",subCount);
  Serial.printf("LostbCount: %d\n",lostCount);
  Serial.println("");
  */
  Serial.println("Cpu Usage");
  Serial.printf("Cpu1: %dms\n",elapsedTime1);
  Serial.printf("Cpu2: %dms\n",elapsedTime2);
  Serial.println("");
  Serial.printf("Uptime: %dsec\n",globaltime);
  Serial.println("-------------------- debug --------------------");
}

// -------------------- task1 --------------------
void Task1code(void*pvParameters){
  for(;;){
    // cpu1 counter and buzzer
    loop1+=1;
    if(loop1==100)loop1=0;

    globaltime=millis()/1000;
    startTime1=millis();

    // process ----------
    // rcv controls
    Trottle=rcvxMsg.trottle;
    Yaw=rcvxMsg.yaw;
    Pitch=rcvxMsg.pitch;
    Roll=rcvxMsg.roll;
    Mode=rcvxMsg.mode;

    // ping from control
    sndxMsg.time1=rcvxMsg.time1;

    // snd ping
    sndxMsg.time2=millis();

    // rcv ping
    if(rcvxMsg.time2<=0)ping=0;
    else ping=millis()-rcvxMsg.time2;

    // safety in case of out of signal
    if(ping>=3000){
      if(millis()-losscount1>=1000){
          losscount1=millis();
          digitalWrite(BUZZER,HIGH);
          delay(50);
          digitalWrite(BUZZER,LOW);
      }
      // stay on position
      Trottle=1500;
      Yaw=1500;
      Pitch=1500;
      Roll=1500;
      Mode=1540; // Loiter mode

      if(millis()-losscount2>=10000){
        if(millis()-losscount1>=500){
          losscount1=millis();
          digitalWrite(BUZZER,HIGH);
          delay(50);
          digitalWrite(BUZZER,LOW);
        }
        // Return to Land
        Mode=1690; // RTL mode
      }

      if(millis()-losscount3>=60000){
        if(millis()-losscount1>=250){
          losscount1=millis();
          digitalWrite(BUZZER,HIGH);
          delay(100);
          digitalWrite(BUZZER,LOW);
        }
      }
    }
    else{
      losscount1=millis();
      losscount2=millis();
      losscount3=millis();
    }

    // write servo
    servo1.write(Trottle);
    servo2.write(Yaw);
    servo3.write(Pitch);
    servo4.write(Roll);
    servo5.write(Mode);

    // percent data
    percentTrottle=mapPercent(Trottle);
    percentYaw=mapPercent(Yaw);
    percentPitch=mapPercent(Pitch);
    percentRoll=mapPercent(Roll);
    mapMode(Mode);

    elapsedTime1=millis()-startTime1;

    // debug ----------
    // serial debug
    //serialDebug(); // enable this for fast debug
    if(millis()-clock1>=200){
      clock1=millis();
      serialDebug();
    }
  }
}

// -------------------- task2 --------------------
void Task2code(void*pvParameters){
  for(;;){
    // cpu2 counter
    loop2+=1;
    if(loop2==100)loop2=0;

    startTime2=millis();

    // serial uart
    serialuart();

    // msg via ESP-NOW
    esp_now_send(targetMac,(uint8_t*)&sndxMsg,sizeof(sndxMsg)); 
    
    elapsedTime2=millis()-startTime2;
  } 
} 

// -------------------- setup --------------------
void setup(){
  // put your setup code here, to run once:
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1,RXD,TXD);

  // int ESP-NOW
  initespnow();

  // buzzer
  pinMode(BUZZER,OUTPUT);

  // servo
  servo1.attach(GPIOTrottle);
  servo2.attach(GPIOYaw);
  servo3.attach(GPIOPitch);
  servo4.attach(GPIORoll);
  servo5.attach(GPIOMode);

  // boot
  initBoot();

  // startup delay
  delay(200);

  // task handler
  xTaskCreatePinnedToCore(Task1code,"cpu1",10000,NULL,2,&cpu1,0);
  xTaskCreatePinnedToCore(Task2code,"cpu2",10000,NULL,1,&cpu2,1);
}

// -------------------- loop --------------------
void loop(){
  // put your main code here, to run repeatedly:
  
}
