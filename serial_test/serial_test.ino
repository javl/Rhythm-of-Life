int pullTimer = 100;
int releaseTimer = 100;

const char startOfNumberDelimiter = '<';
const char endOfNumberDelimiter   = '>';

int settingSpecial = 0;

void setup (){ 
  Serial.begin (115200);
  Serial.println ("Starting ...");
  pinMode(13, OUTPUT);
}

void loop (){
  if (Serial.available ())
    processInput ();
}

void triggerPin(const int pin){
  for(int i=0;i<pin;i++){
    digitalWrite(13, HIGH);
    delay(300); 
    digitalWrite(13, LOW);
    delay(300); 
  }
}

void handleSpecial (const long value){
  if(settingSpecial == 1){
    pullTimer = value;
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
  else if(settingSpecial == 2){
    releaseTimer = value;
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
  settingSpecial = 0;
}

void processInput (){
  static long receivedNumber = 0;

  byte c = Serial.read ();
  switch (c){
  case endOfNumberDelimiter:
    handleSpecial(receivedNumber);
    settingSpecial = 0;
    break;  
  case startOfNumberDelimiter: 
    receivedNumber = 0; 
    break;

  case '0' ... '9': 
    if(!settingSpecial){
      triggerPin(c-'0');
    }
    else{
      receivedNumber *= 10;
      receivedNumber += c - '0';
    }
    break;

  case 'p':
    settingSpecial = 1;
    break;
  case 'r':
    settingSpecial = 2;
    break;
  }
}
