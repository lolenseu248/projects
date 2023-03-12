#define BUTTON_PIN 21 // GIOP21 pin connected to button

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // read the state of the switch/button:
  int buttonState = digitalRead(BUTTON_PIN);

  // print out the button's state
  Serial.println(buttonState);
}
