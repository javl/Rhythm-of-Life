/*======================================================================
 =======================================================================

  _____  _           _   _                        __   _      _  __
 |  __ \| |         | | | |                      / _| | |    (_)/ _|
 | |__) | |__  _   _| |_| |__  _ __ ___     ___ | |_  | |     _| |_ ___
 |  _  /| '_ \| | | | __| '_ \| '_ ` _ \   / _ \|  _| | |    | |  _/ _ \
 | | \ \| | | | |_| | |_| | | | | | | | | | (_) | |   | |____| | ||  __/
 |_|  \_\_| |_|\__, |\__|_| |_|_| |_| |_|  \___/|_|   |______|_|_| \___|
                __/ |
               |___/
 -----------------------------------------------------------------------

 Settings:
 Baudrate: 115200

 Pins:

 Button --------  7
 Power LED ----- 13
 Activity LED -- 12

 Solenoid 1 ----  3
 "        2 ----  4
 "        3 ----  5
 "        4 ----  6
 "        5 ----  9
 "        6 ---- 10
 "        7 ---- 11
 */

//======================================================================
// Define global variables
//======================================================================
#define BTN_PIN 7
#define STATUS_PIN 12
#define POWER_PIN 13

int pins[] = {
  3, 4, 5, 6, 9, 10, 11, 12}; // solenoid pins + status LED

int pullTimer = 30; // amount of time solenoid will be activated

long timers[8] = {
  0, 0, 0, 0, 0, 0, 0, 0}; // timers for solenoids + status LED

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 10;    // the debounce time; increase if the output flickers
int buttonState;            // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

long btnTimer = 0;
long btnDelay = 400;
int btnTimesPressed = 0;
int specialMode = 0;
int menuIndex = 0;
int riderPin = 0;
int riderDirection = 1;
long riderDelay = 120;
long riderTimer = 0;
long flashDelay = 1000;
long flashTimer = 0;

long songDelay = 150;
long songTimer = 0;
int songIndex = 0;

int songScore[] = {
  1, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 9, 9, 9, 1, 9, 1, 9, 1, 9, 9, 9, 1, 9, 1, 9, 2, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

//======================================================================
// Setup and main loop
//======================================================================
void setup (){
  Serial.begin (115200);

  // Set pin modes
  pinMode(BTN_PIN, INPUT);
  digitalWrite(BTN_PIN, HIGH); // enable pulldown resistor for button

  pinMode(STATUS_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH); // turn power LED on

  for(uint8_t i =0; i < 7; i++){
    pinMode(pins[i], OUTPUT); // solenoid pins
  }
}

void loop (){
  if(!Serial){
    digitalWrite(STATUS_PIN, LOW);
  }
  else{
    digitalWrite(STATUS_PIN, HIGH);
  }

  if (Serial.available ()){
    digitalWrite(STATUS_PIN, LOW);
    processInput ();
  }
  checkTimers ();
  checkBtn ();

  // KNIGHRIDER
  if(specialMode == 1){
    if(millis() - riderTimer > riderDelay){
      riderTimer = millis();
      triggerPin(riderPin);
      riderPin += riderDirection;
      if(riderPin < 0){
        riderPin = 1;
        riderDirection = 1;
      }
      else if(riderPin > 6){
        riderPin = 5;
        riderDirection = -1;
      }
    }
  }
  // RANDOM
  else if(specialMode == 2){
    triggerPin(random(7));
  }
  // MOVE RIGHT
  else if(specialMode == 3){
    riderDirection = 1;
    if(millis() - riderTimer > riderDelay){
      riderTimer = millis();
      triggerPin(riderPin);
      riderPin += riderDirection;
      if(riderPin > 6){
        riderPin = 0;
      }
    }
  }
  // MOVE LEFT
  else if(specialMode == 4){
    riderDirection = -1;
    if(millis() - riderTimer > riderDelay){
      riderTimer = millis();
      triggerPin(riderPin);
      riderPin += riderDirection;
      if(riderPin < 0){
        riderPin = 6;
      }
    }
  }
  // FLASH
  else if(specialMode == 5){
    if(millis() - flashTimer > flashDelay){
      flashTimer = millis();
      triggerPin(0);
      triggerPin(1);
      triggerPin(2);
      triggerPin(3);
      triggerPin(4);
      triggerPin(5);
      triggerPin(6);
    }
  }
  else if(specialMode == 6){
    if(millis() - songTimer > songDelay){
      songTimer = millis();
      songIndex++;
      if(songIndex > 36){
        songIndex = 0;
      }
      if(songScore[songIndex] == 1){
        triggerPin(0);
        triggerPin(1);
        triggerPin(2);
      }else if(songScore[songIndex] == 2){
        triggerPin(3);
        triggerPin(4);
        triggerPin(6);
      }
    }
  }
}

//======================================================================
// Check if a timer has expired
//======================================================================
void checkTimers (){
  for(uint8_t index=0;index<8;index++){
    if(timers[index] != 0){
      if(timers[index] < millis()){
        digitalWrite(pins[index], LOW);
        timers[index] = 0;
      }
    }
  }
}

//======================================================================
// Trigger a pin
//======================================================================
void triggerPin(const int index){
  if((  index < 7)){ // solenoids pins or status LED
    if(timers[index] == 0){
      timers[index] = millis()+pullTimer;
      digitalWrite(pins[index], HIGH);
    }
  }
}

//======================================================================
// Process data coming in from serial
//======================================================================
void processInput (){
  static long receivedNumber = 0;
  static bool receivingTimer = 0;

  const byte c = Serial.read ();
  if(specialMode){
    return;
  }
  switch (c){
  case '0' ... '9':
    if(!receivingTimer){
      triggerPin(c-'0');
    }
    else{
      receivedNumber *= 10;
      receivedNumber += c - '0';
    }
    break;
  case '>':
    receivingTimer = 0;
    pullTimer = receivedNumber;
    break;
  case '<':
    receivingTimer = 1;
    receivedNumber = 0;
    break;
  }
}

//======================================================================
// Check state of button
//======================================================================
void checkBtn(){
  //  if(millis() < 2000){ //ignore all inputs during the first 2 seconds
  //    return;
  //  }
  int reading = digitalRead(BTN_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // when button is released
        btnTimesPressed++;
        btnTimer = millis();
      }
    }
  }
  lastButtonState = reading;

  if ((millis() - btnTimer) > btnDelay && btnTimesPressed != 0) {
    if(specialMode != 0){ // exit special mode if active
      specialMode = 0;
      riderPin = 0;
    }
    else{
      if(btnTimesPressed == 1){ // one press, trigger current pin
        triggerByBtn();
      }
      else if(btnTimesPressed == 2){ // two presses, go to next menu option
        menuIndex++;
        if(menuIndex > 7){
          menuIndex = 0;
        }
        triggerByBtn();
      }
      else if(btnTimesPressed == 3){ // three presses, enable special modes if available
        specialMode = menuIndex + 1;
      }
    }
    btnTimesPressed = 0;
  }
}

//======================================================================
// Trigger outputs by pressing button
//======================================================================
void triggerByBtn(){
  if(menuIndex != 7){
    triggerPin(menuIndex);
  }
  else{
    for(uint8_t i =0; i < 7; i++){
      triggerPin(i);
    }
  }
}
