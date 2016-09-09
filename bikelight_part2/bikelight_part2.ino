int rPin1 = 3;
int yPin1 = 5;
int gPin = 6;
int yPin2 = 10;
int rPin2 = 11;
int button = 8;
int dial = A0;
int mode = 0;
int lastButtonState = 0;
int buttonState = 0;
int countMode = 0;
int binaryNum = 0;
float outVoltage = 0;
unsigned long modeTime = 0;
unsigned long offTime = -100;
unsigned long onTime = -100;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long blinkDelay = 50;
unsigned long waveDelay = 100;
unsigned long binaryDelay = 300;


void setup() {
  pinMode(rPin1, OUTPUT);
  pinMode(yPin1, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(yPin2, OUTPUT);
  pinMode(rPin2, OUTPUT);
  pinMode(button, INPUT);
  pinMode(dial, INPUT);
  Serial.begin(9600);
}

void turnAllOn() {
  analogWrite(rPin1, outVoltage);
  analogWrite(yPin1, outVoltage);
  analogWrite(gPin, outVoltage);
  analogWrite(yPin2, outVoltage);
  analogWrite(rPin2, outVoltage); 
}

void turnAllOff() {
  analogWrite(rPin1, LOW);
  analogWrite(yPin1, LOW);
  analogWrite(gPin, LOW);
  analogWrite(yPin2, LOW);
  analogWrite(rPin2, LOW); 
}

void blinkAll(){
  // Default State
  if (offTime == -100 && onTime == -100){
    offTime = millis();
    turnAllOff();
  }
  // Off State has Ended (Begin On State)
  if (offTime != -100 && millis() - offTime > blinkDelay){
    onTime = millis();
    turnAllOn();
    offTime = -100;
  }
  // On State has Ended (Begin default)
  if (onTime != -100 && millis() - onTime > blinkDelay){
    turnAllOff();
    offTime = -100; 
    onTime = -100;
  }
}

void wave() {
  if(millis() - modeTime > waveDelay) {
    modeTime = millis();
    countMode = (countMode+1)%8;
    switch(countMode) {
     case 0:
        waveOn(rPin1);
        break;
     case 1: case 7:
        waveOn(yPin1);
        break;
     case 2: case 6:
        waveOn(gPin);
        break;
     case 3: case 5:
        waveOn(yPin2);
        break;
     case 4:
        waveOn(rPin2);
        break;
    }
  }

}

void waveOn(int pinNumber) {
  turnAllOff();
  analogWrite(pinNumber, outVoltage); 
}

void binaryCount() {
  if(millis() - modeTime > binaryDelay) {
    modeTime = millis();
    countMode = (countMode+1)%32;
    binaryOn(countMode); 
  }
}

void binaryOn(int num) {
   turnAllOff();
   if(num >= 16) {
     analogWrite(rPin1, outVoltage);
     num -= 16;
   }
   if(num >= 8) {
      analogWrite(yPin1, outVoltage);
      num -= 8; 
   }
   if(num >= 4) {
      analogWrite(gPin, outVoltage);
      num -= 4; 
   }
   if(num >= 2) {
      analogWrite(yPin2, outVoltage);
      num -= 2;
   }
   if(num >= 1) {
      analogWrite(rPin2, outVoltage);
      num -= 1; 
   }
}

void loop() {
  int reading = digitalRead(button);
  outVoltage = analogRead(dial)/1023.0 * 255;
  
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if(buttonState) {
        mode = (mode+1)%5;
        modeTime = 0;
        countMode = 0;
        if(mode == 2) {
         offTime = -100;
         onTime = -100; 
        }
      }
    }
  }
  
  switch(mode){
      case 1:
        turnAllOn();
        break;
      case 2:
        blinkAll();
        break;
      case 3:
        wave();
        break;
      case 4:
        binaryCount();
        break;
      default:
        turnAllOff();
        break;
   }
  
  lastButtonState = reading;
}
