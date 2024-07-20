// Transmitter
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#include <MAVLink.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// toggle pinout
#define togSW1 19
#define togSW2 18
#define togSW3 2
#define togSW4 15

// joystick pinout
#define joySW1 25
#define joyX1 33
#define joyY1 32

#define joySW2 26
#define joyX2 34
#define joyY2 35

// potentiometer pinout
#define potenMeter1 36
#define potenMeter2 39

// buffer
#define BUFFER 248

// screen initiation
Adafruit_SSD1306 display(128,64,&Wire,-1);

// -------------------- variables --------------------
// manualvar ----------
// esp-now mymac and targetmac
uint8_t myMac[]={0x40,0x22,0xD8,0x08,0xBB,0x48};
uint8_t targetMac[]={0x40,0x22,0xD8,0x03,0x2E,0x50};

// fixvar ----------
bool espnowEnabled=false;

// peerinfo
esp_now_peer_info_t peerInfo;

// task
TaskHandle_t core0;
TaskHandle_t core1;

// mavlink
uint8_t c;
mavlink_message_t msg;
mavlink_status_t status;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
uint16_t len;

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

// raw data
// toggle inputs
int togSW1State;
int togSW2State;
int togSW3State;
int togSW4State;

// joystick inputs
int joySW1State;
int joyX1Pos;
int joyY1Pos;
int joySW2State;
int joyX2Pos;
int joyY2Pos;

// potentiometer inputs
int potenM1Pos;
int potenM2Pos;

// mapped data
// joystick maps
int joyX1Poss;
int joyY1Poss;
int joyX2Poss;
int joyY2Poss;

// potentiometer maps
int potenM1Poss;
int potenM2Poss;

// joystick1 trottle ajust
int calcTrottle;

// joystick2 speed ajust
int calcSpeed;

// capture trottle
int captureTrottle=1500;

// current trottle
int currentTrottle=1720;

// process data
int Trottle=1500;
int Yaw=1500;
int Pitch=1500;
int Roll=1500;
int Mode=1540;
String Mods;

// percent data
int percentSpeed;
int percentTrottle;
int percentYaw;
int percentPitch;
int percentRoll;

// connection and send data espnow
String comStatus;
int ping;

// send message
typedef struct send_message{
  uint32_t trottle;
  uint32_t yaw;
  uint32_t pitch;
  uint32_t roll;
  uint32_t mode;
  uint64_t time1;
  uint64_t time2;
};
send_message sndxMsg;

// receive message
typedef struct receive_message{
  uint64_t time1;
  uint64_t time2;
};
receive_message rcvxMsg;

// send data
typedef struct send_data{
  uint16_t len;
  uint8_t buf[BUFFER];
};
send_data sndxData;

// receive data
typedef struct receive_data{
  uint16_t len;
  uint8_t buf[BUFFER];
};
receive_data rcvxData;

// -------------------- fuctions --------------------
// processing ----------
// to map value
int setMap(int toMap){
  int subMap=map(toMap,0,4095,0,225); // fix the joystick input because joystick is not centerd to 2048
  int mapValue=map(toMap,0,4095,1000,2225-subMap);

  // default mapping if the joystick centerd to 2048
  //int mapValue=map(toMap,0,4095,1000,2000);
  return mapValue;
}

// maptrottle
void mapTrottle(int toTrottleMap){
  calcTrottle=map(toTrottleMap,1000,2000,1,10);
}

// mapspeed
void mapSpeed(int toSpeedMap){
  calcSpeed=map(toSpeedMap,1000,2000,0,500);
}

// to set official data
// settrottleinmode
int setTrottleInMode(int toTrottleInMode){
  if(toTrottleInMode<=1200)Trottle-=calcTrottle;
  if(toTrottleInMode>=1800)Trottle+=calcTrottle;
  if(Trottle<=1000)Trottle=1000;
  if(Trottle>=2000)Trottle=1800;
  return Trottle;
}

// settrottle
int setTrottle(int toTrottle){
  Trottle=1500;
  if(toTrottle<=1200)Trottle-=calcSpeed;
  if(toTrottle>=1800)Trottle+=calcSpeed;
  return Trottle;
}

// setyaw
int setYaw(int toYaw){
  Yaw=1500;
  if(toYaw<=1200)Yaw-=calcSpeed;
  if(toYaw>=1800)Yaw+=calcSpeed;
  return Yaw;
}

// setpitch
int setPitch(int toPitch){
  Pitch=1500;
  if(toPitch<=1200)Pitch-=calcSpeed;
  if(toPitch>=1800)Pitch+=calcSpeed;
  return Pitch;
}

// setroll
int setRoll(int toRoll){
  Roll=1500;
  if(toRoll<=1200)Roll-=calcSpeed;
  if(toRoll>=1800)Roll+=calcSpeed;
  return Roll;
}

// map to percent
int mapPercent(int toMapPercent){
  int mapValuePercent=map(toMapPercent,1000,2000,0,100);
  return mapValuePercent;
}

// mapmode
void mapMode(int toMode){
  if(toMode>1000)Mods="Stab";
  if(toMode>1231)Mods="PosH";
  if(toMode>1361)Mods="AltH";
  if(toMode>1491)Mods="Loit";
  if(toMode>1621)Mods="RTL ";
  if(toMode>1750)Mods="Land";
}

// esp-now ----------
void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status){
  if(status==ESP_NOW_SEND_SUCCESS)comStatus="ok!";
  else comStatus="bd!";
}

void OnDataRecv(const uint8_t *mac_addr,const uint8_t *incomingData,int data_len){
  if(data_len==sizeof(rcvxData))memcpy(&rcvxData,incomingData,sizeof(rcvxData));
  else memcpy(&rcvxMsg,incomingData,sizeof(rcvxMsg));
}

// startup ----------
// initboot
void initBoot(){
  Serial.println("");
  Serial.println("Botting ...");
  Serial.println("");

  // logo start up
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.print(" Botting ...");
  delay(600);
}

// connection ----------
// init esp-now
void initespnow(){
  if(!espnowEnabled){
    WiFi.mode(WIFI_STA);
    
    // init ESP-NOW
    Serial.println("Initiating ESP-NOW ..");

    if(esp_now_init()!=ESP_OK){
      Serial.println("Error Initializing ESP-NOW");
      espnowEnabled=false;
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
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

    espnowEnabled=true;
  }
  delay(500);
}

// printing ----------
// oled screen
void oledScreen(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("ECOM: ");
  display.println(comStatus);
  display.setCursor(0,0);
  display.print("          PING: ");
  display.print(ping);
  display.println("ms");
  display.setCursor(0,50);
  display.print("Mode: ");
  display.print(Mods);
  display.setCursor(0,10);
  display.print("Trottle:   ");
  display.print(mapPercent(Trottle));
  display.print("%");
  display.setCursor(0,20);
  display.print("Yaw:       ");
  display.print(mapPercent(Yaw));
  display.print("%");
  display.setCursor(0,30);
  display.print("Pitch:     ");
  display.print(mapPercent(Pitch));
  display.print("%");
  display.setCursor(0,40);
  display.print("Roll:      ");
  display.print(mapPercent(Roll));
  display.print("%");
  display.setCursor(0,50);
  display.print("Count: ");
  display.print(loop1);
  display.display();
}

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
  Serial.println("Raw Data");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d\n",joyX1Pos,joyY1Pos,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d\n",joyX2Pos,joyY2Pos,joySW2State);
  Serial.printf("PotentioMeter no.1= %d\n",potenM1Pos);
  Serial.printf("PotentioMeter no.2= %d\n",potenM2Pos);
  Serial.println("");
  Serial.println("Mapped Data");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d\n",joyX1Poss,joyY1Poss,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d\n",joyX2Poss,joyY2Poss,joySW2State);
  Serial.printf("PotentioMeter no.1= %d\n",potenM1Poss);
  Serial.printf("PotentioMeter no.2= %d\n",potenM2Poss);
  Serial.printf("CalcTrottle = %d\n",calcTrottle);
  Serial.printf("CalcSpeed   = %d\n",calcSpeed);
  Serial.println("");
  Serial.println("Switch");
  Serial.printf("JoyStick no.1= %d\n",joySW1State);
  Serial.printf("JoyStick no.2= %d\n",joySW2State);
  Serial.printf("Toggle no.1= %d\n",togSW1State);
  Serial.printf("Toggle no.2= %d\n",togSW2State);
  Serial.printf("Toggle no.3= %d\n",togSW3State);
  Serial.printf("Toggle no.4= %d\n",togSW4State);
  Serial.println("");
  */
  Serial.println("Official Data");
  Serial.printf("Speed: %d%%\n",percentSpeed);
  Serial.printf("Trottle: %d%%\n",percentTrottle);
  Serial.printf("Yaw: %d%%\n",percentYaw);
  Serial.printf("Pitch: %d%%\n",percentPitch);
  Serial.printf("Roll: %d%%\n",percentRoll);
  Serial.printf("Mode: %s\n",Mods);
  Serial.println("");
  Serial.println("Cpu Usage");
  Serial.printf("Core0: %dms\n",elapsedTime1);
  Serial.printf("Core1: %dms\n",elapsedTime2);
  Serial.println("");
  Serial.printf("Uptime: %dsec\n",globaltime);
  Serial.println("-------------------- debug --------------------");
}

// -------------------- task1 --------------------
void Task1code(void*pvParameters){
  for(;;){
    // core0 counter
    loop1+=1;
    if(loop1==100)loop1=0;

    // uptime
    globaltime=millis()/1000;

    // core0 load start
    startTime1=millis();

    // receiving msg ----------
    // rcv ping
    if(rcvxMsg.time1<=0)ping=0;
    else ping=millis()-rcvxMsg.time1;

    // ping from uav
    if(togSW3State==HIGH)sndxMsg.time2=rcvxMsg.time2-3000;
    else if(togSW3State==LOW)sndxMsg.time2=rcvxMsg.time2;

    // procces ----------
    // raw data
    // read toglle input value
    togSW1State=digitalRead(togSW1);
    togSW2State=digitalRead(togSW2);
    togSW3State=digitalRead(togSW3);
    togSW4State=digitalRead(togSW4);

    // read X,Y and SW analog values of joystick no.1
    joySW1State=digitalRead(joySW1);
    joyX1Pos=analogRead(joyX1);
    joyY1Pos=analogRead(joyY1);

    // read X,Y and SW analog values of joystick no.2
    joySW2State=digitalRead(joySW2);
    joyX2Pos=analogRead(joyX2);
    joyY2Pos=analogRead(joyY2);

    // read potentiometer analog values
    potenM1Pos=analogRead(potenMeter1);
    potenM2Pos=analogRead(potenMeter2);

    // mapped data
    // mapped joystick values of joystick no.1
    joyX1Poss=setMap(joyX1Pos);
    joyY1Poss=setMap(joyY1Pos);

    // mapped joystick values of joystick no.2
    joyX2Poss=setMap(joyX2Pos);
    joyY2Poss=setMap(joyY2Pos);

    // mapped potentiometer
    potenM1Poss=setMap(potenM1Pos);
    potenM2Poss=setMap(potenM2Pos);

    // map trottle
    mapTrottle(potenM1Poss);

    // map speed
    mapSpeed(potenM2Poss);

    // prepare for send message
    if(togSW2State==HIGH){
      Yaw=setYaw(joyY1Poss);
      Pitch=setPitch(joyX2Poss);
      Roll=setRoll(joyY2Poss);

      // for the modes
      if(togSW1State==HIGH){
        if(togSW4State==HIGH){
          Trottle=setTrottle(joyX1Poss);
          currentTrottle=Trottle;
          Mode=1555; // Loiter
        }
        else if(togSW4State==LOW){
          Trottle=setTrottle(joyX1Poss);
          currentTrottle=Trottle;
          Mode=1425; // Alt Hold
        }
      }
      else if(togSW1State==LOW){
        if(togSW4State==HIGH){
          Trottle=setTrottleInMode(joyX1Poss);
          if(joySW1State==LOW)Trottle=captureTrottle; // return trottle
          currentTrottle=Trottle;
          Mode=1875; // Land
        }
        else if(togSW4State==LOW){
          Trottle=setTrottle(joyX1Poss);
          currentTrottle=Trottle;
          Mode=1295; // Pos Hold
        }
      }
    }
    else if(togSW2State==LOW){
      Yaw=setYaw(joyY1Poss);
      Pitch=setPitch(joyX2Poss);
      Roll=setRoll(joyY2Poss);

      // for the modes
      if(togSW1State==HIGH){
        if(togSW4State==HIGH){
          Trottle=setTrottle(joyX1Poss);
          currentTrottle=Trottle;
          Mode=1555; // Loiter
        }
        else if(togSW4State==LOW){
          Trottle=setTrottle(joyX1Poss);
          currentTrottle=Trottle;
          Mode=1425; // Alt Hold
        }
      }
      else if(togSW1State==LOW){
        if(togSW4State==HIGH){
          Trottle=setTrottleInMode(joyX1Poss);
          if(joySW1State==LOW)Trottle=captureTrottle; // return trottle
          currentTrottle=Trottle;
          Mode=1875; // Land
        }
        else if(togSW4State==LOW){
          Trottle=currentTrottle;
          if(Trottle<=1100)Yaw=joyY1Poss; // Arming and Disarming
          if(joySW1State==LOW)Trottle=captureTrottle; // return trottle
          if(joySW2State==LOW)captureTrottle=setTrottleInMode(joyX1Poss); // capture trottle
          currentTrottle=setTrottleInMode(joyX1Poss); // set trottle 
          Mode=1115; // Stabilize
        }
      }
    }

    // fix yaw position 
    Yaw=map(Yaw,1000,2000,2000,1000);

    // preparing msg ----------
    // snd controls
    sndxMsg.trottle=Trottle;
    sndxMsg.yaw=Yaw;
    sndxMsg.pitch=Pitch;
    sndxMsg.roll=Roll;
    sndxMsg.mode=Mode;

    // snd ping
    sndxMsg.time1=millis();

    // percent data
    percentSpeed=mapPercent(potenM2Poss);
    percentTrottle=mapPercent(Trottle);
    percentYaw=mapPercent(Yaw);
    percentPitch=mapPercent(Pitch);
    percentRoll=mapPercent(Roll);
    mapMode(Mode);
    
    // sending msg ----------
    // snd msg via ESP-NOW
    esp_now_send(targetMac,(uint8_t*)&sndxMsg,sizeof(sndxMsg));

    delay(2); // run delay

    // core0 load end
    elapsedTime1=millis()-startTime1;

    // debug ----------
    // oled screen
    oledScreen();

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
    // core1 counter
    loop2+=1;
    if(loop2==100)loop2=0;

    // core1 load start
    startTime2=millis();

    // serial uart ----------
    // receive and write
    if(Serial.availableForWrite()>0&&rcvxData.len>0){
      Serial.write(rcvxData.buf,rcvxData.len);
      rcvxData.len=0; // reset to zero
    }

    // read and send
    if(Serial.available()>0){
      while(Serial.available()>0){
        c=Serial.read();
        if(mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
          sndxData.len=mavlink_msg_to_send_buffer(sndxData.buf,&msg);

          // snd msg via ESP-NOW
          esp_now_send(targetMac,(uint8_t*)&sndxData,sizeof(sndxData));
        }
      }
    }
    
    delay(2); // run delay

    // core1 load end
    elapsedTime2=millis()-startTime2;
  } 
}

// -------------------- setup --------------------
void setup(){
  // put your setup code here, to run once:
  // Initialize Serial Monitor
  Serial.begin(115200);

  // initialize OLED display with I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println(F("failed to start SSD1306 OLED"));
    while(1);
  }

  // init ESP-NOW
  initespnow();

  // toggle switch
  pinMode(togSW1,INPUT_PULLUP);
  pinMode(togSW2,INPUT_PULLUP);
  pinMode(togSW3,INPUT_PULLUP);
  pinMode(togSW4,INPUT_PULLUP);
  
  // joystick switch
  pinMode(joySW1,INPUT_PULLUP);
  pinMode(joySW2,INPUT_PULLUP);

  // boot
  initBoot();

  // startup delay
  delay(200);

  // task handler
  xTaskCreatePinnedToCore(Task1code,"core0",10000,NULL,2,&core0,0);
  xTaskCreatePinnedToCore(Task2code,"core1",10000,NULL,1,&core1,1);
}

// -------------------- loop --------------------
void loop(){
  // put your main code here, to run repeatedly:

}
