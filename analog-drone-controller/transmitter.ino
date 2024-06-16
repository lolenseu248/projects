// Transmitter
// @lolenseu
// https:github.com/lolenseu

// -------------------- include and define --------------------
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <MAVLink.h>
#include <esp_now.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//toggle pinout
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

//potentiometer pinout
#define potenMeter1 36
#define potenMeter2 39

// websoket
WebServer server(80);

// screen initiation
Adafruit_SSD1306 display(128,64,&Wire,-1);

// -------------------- variables --------------------
// manualvar ----------
// ssdid and pass of wifi
const char* ssid="apm2.8-hexa";
const char* password="12345678";

// static ip
IPAddress local_IP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// esp-now mymac and targetmac
uint8_t myMac[]={0x40,0x22,0xD8,0x08,0xBB,0x48};
uint8_t targetMac[]={0x40,0x22,0xD8,0x03,0x2E,0x50};

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

// current trottle
int currentTrottle=1720;

// sending process data
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

// send_message
typedef struct send_message{
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
send_message sndxMsg;

// recive_message
typedef struct receive_message{
  int time1;
  int time2;
  uint16_t len;
  uint8_t buf[128];
};
receive_message rcvxMsg;

// -------------------- fuctions --------------------
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
  delay(300);
}

// connection ----------
// init wifi
void initwifi(){
  WiFi.softAPConfig(local_IP,gateway,subnet);
  WiFi.softAP(ssid,password);

  // print the IP address
  IPAddress IP=WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  delay(500);
}

// init esp-now
void initespnow(){
  //WiFi.mode(WIFI_STA);
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
  if(Serial.availableForWrite()>0){
    Serial.write(rcvxMsg.buf,rcvxMsg.len);
  }

  // heartbeat
  if(millis()-lastHeartbeatTime>=1000){
    lastHeartbeatTime=millis();
    mavlink_msg_heartbeat_pack(1,MAV_COMP_ID_AUTOPILOT1,&msg,MAV_TYPE_QUADROTOR,MAV_AUTOPILOT_GENERIC,MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,0,MAV_STATE_STANDBY);
    sndxMsg.len=mavlink_msg_to_send_buffer(sndxMsg.buf,&msg);
  }

  // serial uart read and send
  else{
    while(Serial.available()>0){
      uint8_t c=Serial.read();
      if (mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
        sndxMsg.len=mavlink_msg_to_send_buffer(sndxMsg.buf,&msg);
      }
    }
  }
}

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
  if(toTrottle<=1200)Trottle=Trottle-=5;
  if(toTrottle>=1800)Trottle=Trottle+=5;
  if(Trottle<=1000)Trottle=1000;
  if(Trottle>=2000)Trottle=1800;
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

// printing ----------
// webcontent
void handleRoot(){
  String html = "<html><head>";
  html += "<meta http-equiv='refresh' content='0.5'>";
  html += "<style>";
  html += "body { text-align: center; }";
  html += "h1 { font-size: 60px; }";
  html += "h2 { font-size: 50px; }";
  html += "p { font-size: 50px; }";
  html += ".flex-container { display: flex; justify-content: center; margin: 0 auto; padding-left: 15%; padding-right: 15% }";
  html += ".left { flex: 1; text-align: left; font-size: 50px; }";
  html += ".right { flex: 1; text-align: right; font-size: 50px; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>apm2.8-hexa</h1>";
  html += "<h2>ESP-NOW</h2>";
  html += "<div class='flex-container'>";
  html += "<div class='left'>Com Status: " + String(comStatus) + "</div>";
  html += "<div class='right'>ping: " + String(ping) + "ms</div>";
  html += "</div>";
  html += "<h2>Official Data</h2>";
  html += "<div class='flex-container'>";
  html += "<div class='left'>Speed: " + String(percentSpeed) + "%</div>";
  html += "<div class='right'>Mod: " + String(Mods) + "</div>";
  html += "</div>";
  html += "<div class='flex-container'>";
  html += "<div class='left'>Trottle: " + String(percentTrottle) + "%</div>";
  html += "<div class='right'>Yaw: " + String(percentYaw) + "%</div>";
  html += "</div>";
  html += "<div class='flex-container'>";
  html += "<div class='left'>Pitch: " + String(percentPitch) + "%</div>";
  html += "<div class='right'>Roll: " + String(percentRoll) + "%</div>";
  html += "</div>";
  html += "<h2>Cpu Usage</h2>";
  html += "<div class='flex-container'>";
  html += "<div class='left'>Cpu1: " + String(elapsedTime1) + "ms</div>";
  html += "<div class='right'>Cpu2: " + String(elapsedTime2) + "ms</div>";
  html += "</div>";
  html += "<p>Uptime: " + String(globaltime) + "sec</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


// oled screen setup1
void oledScreen1(){
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
  display.print("JSNo.1: ");
  display.print("X=");
  display.print(joyX1Poss);
  display.print("\tY=");
  display.print(joyY1Poss);
  display.setCursor(0,20);
  display.print("JSNo.2: ");
  display.print("X=");
  display.print(joyX2Poss);
  display.print("\tY=");
  display.print(joyY2Poss);
  display.setCursor(0,30);
  display.print("PMNo.1: ");
  display.print(potenM1Poss);
  display.setCursor(0,40);
  display.print("PMNo.2: ");
  display.print(potenM2Poss);  
  display.setCursor(0,50);
  display.print("Count: ");
  display.print(loop1);
  display.display();
}

// oled screen setup2
void oledScreen2(){
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
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
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
  Serial.printf("Cpu1: %dms\n",elapsedTime1);
  Serial.printf("Cpu2: %dms\n",elapsedTime2);
  Serial.println("");
  Serial.printf("Uptime: %dsec\n",globaltime);
  Serial.println("-------------------- debug --------------------");
}

// -------------------- task1 --------------------
void Task1code(void*pvParameters){
  for(;;){
    // cpu1 counter
    loop1+=1;
    if(loop1==100)loop1=0;

    globaltime=millis()/1000;
    startTime1=millis();

    // webserver
    server.handleClient();

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

    // mapped joystic values of joystic no.1
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

    // snd controls
    sndxMsg.trottle=Trottle;
    sndxMsg.yaw=Yaw;
    sndxMsg.pitch=Pitch;
    sndxMsg.roll=Roll;
    sndxMsg.mode=Mode;

    // snd ping
    if(togSW3State==HIGH)sndxMsg.time1=0;
    if(togSW3State==LOW)sndxMsg.time1=millis();
    
    // rcv ping
    if(0>=rcvxMsg.time1)ping=0;
    else ping=rcvxMsg.time1-millis();

    // ping from uav
    sndxMsg.time2=rcvxMsg.time2;

    // percent data
    percentSpeed=mapPercent(potenM2Poss);
    percentTrottle=mapPercent(Trottle);
    percentYaw=mapPercent(Yaw);
    percentPitch=mapPercent(Pitch);
    percentRoll=mapPercent(Roll);
    mapMode(Mode);

    elapsedTime1=millis()-startTime1;

    // debug ----------
    // oled screen
    // oleddisplay1
    if(togSW3State==HIGH)oledScreen1();

    // oleddisplay2
    else if(togSW3State==LOW)oledScreen2();

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

  // initialize OLED display with I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println(F("failed to start SSD1306 OLED"));
    while(1);
  }

  // init wifi
  initwifi();

  // init ESP-NOW
  initespnow();

  // define the root URL ("/") handler
  server.on("/",handleRoot);

  // start the server
  server.begin();

  // toggle switch
  pinMode(togSW1,INPUT_PULLUP);
  pinMode(togSW2,INPUT_PULLUP);
  pinMode(togSW3,INPUT_PULLUP);
  pinMode(togSW4,INPUT_PULLUP);
  
  // joystick switch
  pinMode(joySW1,INPUT);
  pinMode(joySW2,INPUT);

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
