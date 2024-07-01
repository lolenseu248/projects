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

// joystic pinout
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
#define BUFFER 128

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

// joystic inputs
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
// joystic maps
int joyX1Poss;
int joyY1Poss;
int joyX2Poss;
int joyY2Poss;

// potentiometer maps
int potenM1Poss;
int potenM2Poss;

// joystic2 speed ajust
int calcLow;
int calcHigh;

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
  uint16_t len;
  uint8_t buf[BUFFER];
};
send_message sndxMsg;

// receive message
typedef struct receive_message{
  uint64_t time1;
  uint64_t time2;
  uint16_t len;
  uint8_t buf[BUFFER];
};
receive_message rcvxMsg;

// -------------------- fuctions --------------------
// processing ----------
// to map value
int setMap(int toMap){
  int subMap=map(toMap,0,4095,0,225); // fix the joystic input because joystic is not centerd to 2048
  int mapValue=map(toMap,0,4095,1000,2225-subMap);

  // default mapping if the joystic centerd to 2048
  //int mapValue=map(toMap,0,4095,1000,2000);
  return mapValue;
}

// mapspeed
void mapSpeed(int toSpeed){
  calcLow=1500-map(toSpeed,1000,2000,0,500);
  calcHigh=1500+map(toSpeed,1000,2000,0,500);
}

// to set official data
// settrottle
int setTrottle(int toTrottle){
  if(Trottle<=1500||Trottle>=1800){
    if(toTrottle<=1200)Trottle=Trottle-=5;
    if(toTrottle>=1800)Trottle=Trottle+=5;
  }
  else {
    if(toTrottle<=1200)Trottle=Trottle-=2;
    if(toTrottle>=1800)Trottle=Trottle+=2;
  }
  if(Trottle<=1000)Trottle=1000;
  if(Trottle>=2000)Trottle=1700;
  return Trottle;
}

// settrottleinmode
int setTrottleInMode(int toTrottle){
  Trottle=1500;
  if(toTrottle<=1200)Trottle=1350;
  if(toTrottle>=1800)Trottle=1650;
  return Trottle;
}

// setyaw
int setYaw(int toYaw){
  if(toYaw==1500,toYaw>=1450,toYaw<=1550)Yaw=1500;
  if(toYaw<=1200)Yaw=calcLow;
  if(toYaw>=1800)Yaw=calcHigh;
  return Yaw;
}

// setpitch
int setPitch(int toPitch){
  if(toPitch==1500,toPitch>=1450,toPitch<=1550)Pitch=1500;
  if(toPitch<=1200)Pitch=calcLow;
  if(toPitch>=1800)Pitch=calcHigh;
  return Pitch;
}

// setroll
int setRoll(int toRoll){
  if(toRoll==1500,toRoll>=1450,toRoll<=1550)Roll=1500;
  if(toRoll<=1200)Roll=calcLow;
  if(toRoll>=1800)Roll=calcHigh;
  return Roll;
}

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
    esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(OnDataRecv));

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

    // read X,Y and SW analog values of joystic no.1
    joySW1State=digitalRead(joySW1);
    joyX1Pos=analogRead(joyX1);
    joyY1Pos=analogRead(joyY1);

    // read X,Y and SW analog values of joystic no.2
    joySW2State=digitalRead(joySW2);
    joyX2Pos=analogRead(joyX2);
    joyY2Pos=analogRead(joyY2);

    // read potentiometer analog values
    potenM1Pos=analogRead(potenMeter1);
    potenM2Pos=analogRead(potenMeter2);

    // mapped data
    // mapped joystic values of joystic no.1
    joyX1Poss=setMap(joyX1Pos);
    joyY1Poss=setMap(joyY1Pos);

    // mapped joystic values of joystic no.2
    joyX2Poss=setMap(joyX2Pos);
    joyY2Poss=setMap(joyY2Pos);

    // mapped potentiometer
    potenM1Poss=setMap(potenM1Pos);
    potenM2Poss=setMap(potenM2Pos);

    // map mode to string
    mapSpeed(potenM2Poss);

    // prepare for send message
    if(togSW2State==HIGH){
      Yaw=setYaw(joyY1Poss);
      Pitch=setPitch(joyX2Poss);
      Roll=setRoll(joyY2Poss);

      // for the modes
      if(togSW1State==HIGH){
        if(togSW4State==HIGH){
          Trottle=setTrottleInMode(joyX1Poss);
          Mode=1550; // Loiter
        }
        else if(togSW4State==LOW){
          Trottle=setTrottleInMode(joyX1Poss);
          Mode=1400; // Alt Hold
        }
      }
      else if(togSW1State==LOW){
        if(togSW4State==HIGH){
          Trottle=setTrottleInMode(joyX1Poss);
          Mode=1820; // Land
        }
        else if(togSW4State==LOW){
          Trottle=currentTrottle;

          // return trottle
          if(joySW1State==LOW){
            Trottle=captureTrottle;
          }

          // capture trottle
          if(joySW2State==LOW){
            captureTrottle=setTrottle(joyX1Poss);
          }
          
          currentTrottle=setTrottle(joyX1Poss); // setTrottle only on knob or stab
          Mode=potenM1Poss; // Fix by knob
        }
      }
    }
    else if(togSW2State==LOW){
      currentTrottle=joyX1Poss;
      Trottle=currentTrottle;
      Yaw=joyY1Poss;
      Pitch=joyX2Poss;
      Roll=joyY2Poss;

      // for the modes
      if(togSW1State==HIGH){
        if(togSW4State==HIGH){
          Mode=1550; // Loiter
        }
        else if(togSW4State==LOW){
          Mode=1400; // Alt Hold
        }
      }
      else if(togSW1State==LOW){
        if(togSW4State==HIGH){
          Mode=1820; // Land
        }
        else if(togSW4State==LOW){
          Mode=1100; // Fix Stabilize
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

    delay(10); // run delay

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
    if(rcvxMsg.len>0){
      Serial.write(rcvxMsg.buf,rcvxMsg.len);
      rcvxMsg.len=0; // reset to zero
    }
    
    // heartbeat
    if(millis()-lastHeartbeatTime>=1000){
      lastHeartbeatTime=millis();
      mavlink_msg_heartbeat_pack(1,MAV_COMP_ID_AUTOPILOT1,&msg,MAV_TYPE_QUADROTOR,MAV_AUTOPILOT_GENERIC,MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,0,MAV_STATE_STANDBY);
      sndxMsg.len=mavlink_msg_to_send_buffer(sndxMsg.buf,&msg);
    }

    // read and send
    else{
      while(Serial.available()){
        c=Serial.read();
        if(mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
          len=mavlink_msg_to_send_buffer(sndxMsg.buf,&msg);
        }
      }
    }

    if(len>0){
      sndxMsg.len=len;
      len=0;
    }
    else sndxMsg.len=0;

    // sending msg ----------
    // snd msg via ESP-NOW
    esp_now_send(targetMac,(uint8_t*)&sndxMsg,sizeof(sndxMsg));

    delay(10); // run delay

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
