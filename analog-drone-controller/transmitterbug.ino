
// ---------- include and define ----------

#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// oled screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino resetpin)

// screen initiation
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// joystic pinoutss
#define joyX1 33
#define joyY1 32
#define joySW1 25

#define joyX2 34
#define joyY2 35
#define joySW2 26

//potentiometer pinouts
#define potenMeter1 36
#define potenMeter2 39

//toggle switchs
#define togSW1 19
#define togSW2 18
#define togSW3 15
#define togSW4 2


// ---------- variables ----------

// wificonfig
const char* ssid = "lolenseu";
const char* password = "@lolenseu24!";

// com server
String serverName = "https://blynk.cloud/external/api/update?token=Z28VmfqlAHMfu1cQrnFKYZ5RFfK0lyXP&v0=";


// raw data
// joystic inputs
int joyX1Pos;
int joyY1Pos;
int joySW1State;

int joyX2Pos;
int joyY2Pos;
int joySW2State;

// potentiometer inputs
int potenM1Pos;
int potenM2Pos;

// toggle
int togSW1State;
int togSW2State;
int togSW3State;
int togSW4State;
int togSW5State;
int togSW6State;


// mapped data
// joystic maps
int joyX1Poss;
int joyY1Poss;

int joyX2Poss;
int joyY2Poss;

// potentiometer maps
int potenM1Poss;
int potenM2Poss;

// variable to store if sending data was successful
String success;

// storage message
typedef struct struct_message {
    int trottle;
    int yaw;
    int pitch;
    int roll;
    int mode;
} struct_message;

// counter
int counter = 0;


// sending process data
int Trottle = 0;
int Yaw = 1500;
int Pitch = 1500;
int Roll = 1500;
int Mode = 1000;
String Mods;


// ---------- fuctions ----------

// initwifi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(WiFi.localIP());
}

// inticom
int initCom(String sendData) {
  HTTPClient http;
  String serverPath = serverName + "0x" + sendData;
  http.begin(serverPath);
  int httpCode = http.GET();                                                        
  if (httpCode > 0) {
    String payload = http.getString(); 
    Serial.println(payload);
  }

  http.end();
  return 0;
}

// to map value
int setMap(int toMap) {
  int mapValue = map(toMap, -360, 4095, 1000, 2000);
  return mapValue;
}

// map to percent
int mapPercent(int toMapPercent) {
  int mapValuePercent = map(toMapPercent, 1000, 2000, 0, 100);
  return mapValuePercent;
}

// to set official data
//trottle
int setTrottle(int toTrottle) {
  if (toTrottle <= 1200) Trottle = Trottle -= 5;
  if (toTrottle >= 1800) Trottle = Trottle += 5;
  if (Trottle <= 1000) Trottle = 1000;
  if (Trottle >= 2000) Trottle = 1800;
  return Trottle;
}

//yaw
int setYaw(int toYaw) {
  if (toYaw == 1500, toYaw >= 1450, toYaw <= 1550) Yaw = 1500;
  if (toYaw <= 1200) Yaw = 1400;
  if (toYaw >= 1800) Yaw = 1600;
  return Yaw;
}

//pitch
int setPitch(int toPitch) {
  if (toPitch == 1500, toPitch >= 1450, toPitch <= 1550) Pitch = 1500;
  if (toPitch <= 1200) Pitch = 1400;
  if (toPitch >= 1800) Pitch = 1600;
  return Pitch;
}

//roll
int setRoll(int toRoll) {
  if (toRoll == 1500, toRoll >= 1450, toRoll <= 1550) Roll = 1500;
  if (toRoll <= 1200) Roll = 1400;
  if (toRoll >= 1800) Roll = 1600;
  return Roll;
}

//mode
void setMode(int toMode) {
  if (toMode == 1180, toMode >= 1130, toMode <= 1220) Mods = "Stable";
  if (toMode == 1280, toMode >= 1330, toMode <= 1420) Mods = "Alt. H";
  if (toMode == 1480, toMode >= 1430, toMode <= 1520) Mods = "Loiter";
  if (toMode == 1580, toMode >= 1530, toMode <= 1620) Mods = "Guide";
  if (toMode == 1780, toMode >= 1630, toMode <= 1720) Mods = "ReHome";
  if (toMode == 1880, toMode >= 1730, toMode <= 1820) Mods = "Land";
}

// serial debug
void serialDebug() {
  Serial.printf("\n");
  Serial.printf("-------------------- debug --------------------");

  Serial.print("\nWiFi\n");
  Serial.printf("RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.printf("\n");

  Serial.printf("Raw Data \n");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d \n",joyX1Pos,joyY1Pos,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d \n",joyX2Pos,joyY2Pos,joySW2State);
  Serial.printf("PotentioMeter no.1= %f \n",potenM1Pos);
  Serial.printf("PotentioMeter no.2= %f \n",potenM2Pos);
  Serial.printf("\n");
  
  Serial.printf("Mapped Data \n");
  Serial.printf("JoyStick no.1 X= %d, Y= %d, Sw= %d \n",joyX1Poss,joyY1Poss,joySW1State);
  Serial.printf("JoyStick no.2 X= %d, Y= %d, Sw= %d \n",joyX2Poss,joyY2Poss,joySW2State);
  Serial.printf("PotentioMeter no.1= %f \n",potenM1Poss);
  Serial.printf("PotentioMeter no.2= %f \n",potenM2Poss);
  Serial.printf("\n");
  
  Serial.printf("Switch \n");
  Serial.printf("JoyStick no.1= %d \n",joySW1State);
  Serial.printf("JoyStick no.2= %d \n",joySW2State);
  Serial.printf("Toggle no.1= %d \n",togSW1State);
  Serial.printf("Toggle no.2= %d \n",togSW2State);
  Serial.printf("Toggle no.3= %d \n",togSW3State);
  Serial.printf("Toggle no.4= %d \n",togSW4State);
  Serial.printf("Toggle no.3= %d \n",togSW5State);
  Serial.printf("Toggle no.4= %d \n",togSW6State);
  Serial.printf("\n");

  Serial.printf("Official Data \n");
  Serial.printf("Trottle: %d%\n",Trottle);
  Serial.printf("Yaw: %d%\n",Yaw);
  Serial.printf("Pitch %d%\n",Pitch);
  Serial.printf("Roll %d%\n",Roll);
  Serial.printf("Mode %d%\n",Mode);
  Serial.printf("\n");
  
  Serial.printf("Counter= %d \n",counter);

  Serial.printf("-------------------- debug --------------------");
  Serial.printf("\n");
}


// oled screen setup1
void oledScreen1() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("RSSI: ");
  display.print(WiFi.RSSI());

  display.setCursor(0, 0);
  display.print("          Mode: ");
  display.print(Mods);
  
  
  display.setCursor(0, 10);
  display.print("Trottle: ");
  display.print(mapPercent(Trottle));
  display.print("%");

  display.setCursor(0, 20);
  display.print("Yaw:     ");
  display.print(mapPercent(Yaw));
  display.print("%");
  
  display.setCursor(0, 30);
  display.print("Pitch:   ");
  display.print(mapPercent(Pitch));
  display.print("%");

  display.setCursor(0, 40);
  display.print("Roll:    ");
  display.print(mapPercent(Roll));
  display.print("%");
  
  display.setCursor(0, 50);
  display.print("Counter: ");
  display.print(counter);
  display.display();  
}


// oled screen setup2
void oledScreen2() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("RSSI: ");
  display.print(WiFi.RSSI());

  display.setCursor(0, 0);
  display.print("          Mode: ");
  display.print(Mods);
  
  display.setCursor(0, 10);
  display.print("JSNo.1: ");
  display.print("X=");
  display.print(joyX1Poss);
  display.print("\tY=");
  display.print(joyY1Poss);
  
  display.setCursor(0, 20);
  display.print("JSNo.2: ");
  display.print("X=");
  display.print(joyX2Poss);
  display.print("\tY=");
  display.print(joyY2Poss);

  display.setCursor(0, 30);
  display.print("PMNo.1: ");
  display.print(potenM1Poss);

  display.setCursor(0, 40);
  display.print("PMNo.2: ");
  display.print(potenM2Poss);
  
  display.setCursor(0, 50);
  display.print("Counter: ");
  display.print(counter);
  display.display();
}


// ---------- setup ----------

void setup() {
  Serial.begin(115200);
  // initialize OLED display with I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  // initWiFi
  initWiFi();

  // intCom
  initCom("00");

  // joystick switch
  pinMode(joySW1, INPUT);
  pinMode(joySW2, INPUT);

  // toggle switch
  pinMode(togSW1, INPUT_PULLUP);
  pinMode(togSW2, INPUT_PULLUP);
  pinMode(togSW3, INPUT_PULLUP);
  pinMode(togSW4, INPUT_PULLUP);


  // display
  display.clearDisplay();
  display.display();
}


// ---------- loop ----------

void loop() {
  // put your main code here, to run repeatedly:

  // counter and buzzer
  if (counter == 100) counter = 0;
  counter += 1;
  
  
  // receive message
  
  
  
  // raw data
  // read X,Y and SW analog values of joystic no.1
  joyX1Pos = analogRead(joyX1);
  joyY1Pos = analogRead(joyY1);
  joySW1State = digitalRead(joySW1);

  // read X,Y and SW analog values of joystic no.2
  joyX2Pos = analogRead(joyX2);
  joyY2Pos = analogRead(joyY2);
  joySW2State = digitalRead(joySW2);

  // read potentiometer analog values
  potenM1Pos = analogRead(potenMeter1);
  potenM2Pos = analogRead(potenMeter2);

  // read toglle input value
  togSW1State = digitalRead(togSW1);
  togSW2State = digitalRead(togSW2);
  togSW3State = digitalRead(togSW3);
  togSW4State = digitalRead(togSW4);


  // mapped data
  // mapped joystic values of joystic no.1
  joyX1Poss = setMap(joyX1Pos);
  joyY1Poss = setMap(joyY1Pos);

  // mapped joystic values of joystic no.1
  joyX2Poss = setMap(joyX2Pos);
  joyY2Poss = setMap(joyY2Pos);

  // mapped potentiometer
  potenM1Poss = setMap(potenM1Pos);
  potenM2Poss = setMap(potenM2Pos);
  

  // prepare for send message
  if (togSW1State == 1) {
    Trottle = setTrottle(joyX1Poss);
    Yaw = setYaw(joyY1Poss);
    Pitch = setPitch(joyX2Poss);
    Roll = setRoll(joyY2Poss);
    setMode(potenM1Poss);
  }

  if (togSW2State == 1) {
    potenM1Poss = 1000;
    Trottle = joyX1Poss;
    Yaw = joyY1Poss;
    Pitch = joyX2Poss;
    Roll = joyY2Poss;
    Mode = potenM1Poss;
    setMode(potenM1Poss);
  }
  


  // send message via request
  


  // srial debug
  serialDebug();

  // oled screen
  // oleddisplay1
  if (togSW3State == 1) oledScreen1();

  // oleddisplay2
  if (togSW4State == 1) oledScreen2();
  

  // delay
  //delay(10); // run delay
  //delay(100); // test delay
  //delay(1000); // debug delay
  //delay(60000); // stop delay
}
