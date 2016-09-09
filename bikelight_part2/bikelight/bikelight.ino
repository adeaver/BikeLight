int rPin1 = 3;
int yPin1 = 5;
int gPin = 6;
int yPin2 = 10;
int rPin2 = 11;
int button = 8;
int mode = 0;
int lastButtonState = 0;
int buttonState = 0;
int waveMode = 0;
unsigned long waveTime = 0;
unsigned long offTime = -100;
unsigned long onTime = -100;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long blinkDelay = 50;
unsigned long waveDelay = 100;

void setup() {
  pinMode(rPin1, OUTPUT);
  pinMode(yPin1, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(yPin2, OUTPUT);
  pinMode(rPin2, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void turnAllOn() {
  digitalWrite(rPin1, HIGH);
  digitalWrite(yPin1, HIGH);
  digitalWrite(gPin, HIGH);
  digitalWrite(yPin2, HIGH);
  digitalWrite(rPin2, HIGH); 
}

void turnAllOff() {
  digitalWrite(rPin1, LOW);
  digitalWrite(yPin1, LOW);
  digitalWrite(gPin, LOW);
  digitalWrite(yPin2, LOW);
  digitalWrite(rPin2, LOW); 
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
  if(millis() - waveTime > waveDelay) {
    waveTime = millis();
    waveMode = (waveMode+1)%5;
    switch(waveMode) {
     case 0:
        waveOn(rPin1);
        break;
     case 1:
        waveOn(yPin1);
        break;
     case 2:
        waveOn(gPin);
        break;
     case 3:
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
  digitalWrite(pinNumber, HIGH); 
}

void loop() {
  int reading = digitalRead(button);
  
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if(buttonState) {
        mode = (mode+1)%3;
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
      default:
        turnAllOff();
        break;
   }
  
  lastButtonState = reading;
}
