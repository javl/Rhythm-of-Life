/*
 Serial:            115200
 Solenoid:          digital pin 6
 Indicator LED:     pin 13 (on-board LED)
 
 Script takes the following serial input:
 q: pullTimer up
 a: pullTimer down
 w: releaseTimer up
 s: releaseTimer down
 e: betweenTimer up
 d: betweenTimer down
 r: reset to defaults
 */

#define SOLENOIDPIN 3
#define LEDPIN 13
#define INIT_PULLTIMER 20
#define INIT_RELEASETIMER 65
#define INIT_BETWEENTIMER 0
#define TIMER_STEP 5

long pullTimer = INIT_PULLTIMER;
long releaseTimer = INIT_RELEASETIMER;
long betweenTimer = INIT_BETWEENTIMER;

void setup() {
  pinMode(SOLENOIDPIN, OUTPUT);
  digitalWrite(SOLENOIDPIN, LOW);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Solenoid on pin: ");
  Serial.println(SOLENOIDPIN);
  Serial.print("pullTimer: ");
  Serial.println(INIT_PULLTIMER);
  Serial.print("releaseTimer: ");
  Serial.println(INIT_RELEASETIMER);
  Serial.print("betweenTimer: ");
  Serial.println(INIT_BETWEENTIMER);
}

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    switch(inChar){
    case 'q':
      pullTimer+=TIMER_STEP;
      Serial.print("Increase pullTimer: ");
      Serial.println(pullTimer);
      break; 
    case 'a':
      pullTimer-=TIMER_STEP;
      if(pullTimer < 0){ 
        pullTimer = 0; 
      };
      Serial.print("Decrease pullTimer: ");
      Serial.println(pullTimer);
      break; 
    case 'w':
      releaseTimer+=TIMER_STEP;
      Serial.print("Increase releaseTimer: ");
      Serial.println(releaseTimer);
      break; 
    case 's':
      releaseTimer-=TIMER_STEP;
      if(pullTimer < 0){ 
        pullTimer = 0; 
      };
      Serial.print("Decrease releaseTimer: ");
      Serial.println(releaseTimer);
      break; 
    case 'e':
      betweenTimer+=TIMER_STEP;
      Serial.print("Increase betweenTimer: ");
      Serial.println(betweenTimer);
      break; 
    case 'd':
      betweenTimer-=TIMER_STEP;
      if(betweenTimer <0){
        betweenTimer = 0; 
      }
      Serial.print("Increase betweenTimer: ");
      Serial.println(betweenTimer);
      break; 
    case 'r':
      Serial.print("Reset to defaults:");
      Serial.print(INIT_PULLTIMER);
      Serial.print(", ");
      Serial.print(INIT_RELEASETIMER);
      Serial.print(", ");
      Serial.println(INIT_BETWEENTIMER);
      pullTimer = INIT_PULLTIMER;
      releaseTimer = INIT_RELEASETIMER;
      betweenTimer = INIT_BETWEENTIMER;
      digitalWrite(SOLENOIDPIN, LOW);
      digitalWrite(LEDPIN, LOW);
      break;
    case 'g':
      digitalWrite(SOLENOIDPIN, HIGH);
      digitalWrite(LEDPIN, HIGH);
      delay(pullTimer);
      digitalWrite(SOLENOIDPIN, LOW);
      digitalWrite(LEDPIN, LOW);
      delay(releaseTimer);
      delay(betweenTimer);
    default:
      // ignore other entries
      break;
    }
  }
}

















