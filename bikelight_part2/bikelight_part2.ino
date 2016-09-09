// Define the pins, these are all PWM pins
int rPin1 = 3;
int yPin1 = 5;
int gPin = 6;
int yPin2 = 10;
int rPin2 = 11;

// Input pins
int button = 8;
int dial = A0;

// State variables
int mode = 0; // current mode of bike light
int lastButtonState = 0; // previous button state
int buttonState = 0; // current button state
int countMode = 0; // number used to assign modes in wave and binary
int binaryNum = 0; // current binary number
float outVoltage = 0; // brightness

// Time Variables 
unsigned long modeTime = 0; // time in a given mode during wave and binary
unsigned long offTime = -100; // time LEDs are off during blink
unsigned long onTime = -100; // time LEDs are on during blink
unsigned long lastDebounceTime = 0; // time since button last changed

// Delays
unsigned long debounceDelay = 50; // delay between button presses
unsigned long blinkDelay = 50; // delay between blinks
unsigned long waveDelay = 100; // delay between LEDs turning on during wave
unsigned long binaryDelay = 300; // delay between increments during binary

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
        // If the button is pressed, change modes
        resetMode();
      }
    }
  }
  
  // run the current mode
  runMode();
  
  lastButtonState = reading;
}

// Increment the mode and start in default states
void resetMode() {
  // increment mode between 0-4
  mode = (mode+1)%5;
  
  // start in default mode for wave and binary
  modeTime = 0;
  countMode = 0;
  
  // if the mode is blink, reset the values for on and off time
  if(mode == 2) {
   offTime = -100;
   onTime = -100; 
  }  
}

// runs the appropriate function given the mode
void runMode() {
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
}

// This method turns all of the LEDs on
void turnAllOn() {
  analogWrite(rPin1, outVoltage);
  analogWrite(yPin1, outVoltage);
  analogWrite(gPin, outVoltage);
  analogWrite(yPin2, outVoltage);
  analogWrite(rPin2, outVoltage); 
}

// This method turns all of the LEDs off
void turnAllOff() {
  analogWrite(rPin1, LOW);
  analogWrite(yPin1, LOW);
  analogWrite(gPin, LOW);
  analogWrite(yPin2, LOW);
  analogWrite(rPin2, LOW); 
}

// This method blinks all of the lights
void blinkAll(){
  // Default State
  if (offTime == -100 && onTime == -100){
    // if both counters are off
    
    offTime = millis(); // start counting the time spent in off state
    turnAllOff(); 
  }
  // Off State has Ended (Begin On State)
  if (offTime != -100 && millis() - offTime > blinkDelay) { 
    // if offTime counter has started and expired
    
    onTime = millis(); // begin onTime counter
    turnAllOn(); 
    offTime = -100; // turn off off time counter
  }
  // On State has Ended (Begin default)
  if (onTime != -100 && millis() - onTime > blinkDelay){
    // if the onTime counter has started and expired
    
    turnAllOff();
    
    // turn off both counters
    offTime = -100; 
    onTime = -100;
  }
  
  /* Try this:
  
  if(millis() - modeTime > blinkDelay) {
     modeTime = millis();
     
     if(modeCount == 0) {
        turnAllOff(); 
     } else {
       turnAllOn();
     }
     
     modeCount = (modeCount+1)%2;
  }  
  */
}

// This method blinks all the lights sequentially
void wave() {
  if(millis() - modeTime > waveDelay) {
    // if the timer between lights has expired
    
    modeTime = millis(); // set the timer
    
    countMode = (countMode+1)%8; // change the mode
    
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

// This methods turns the correct pin on for the wave
void waveOn(int pinNumber) {
  turnAllOff();
  analogWrite(pinNumber, outVoltage); 
}

// This method counts to 31 in binary using the LEDs
void binaryCount() {
  if(millis() - modeTime > binaryDelay) {
    modeTime = millis();
    countMode = (countMode+1)%32;
    binaryOn(countMode); 
  }
}

// This method takes in a number and determines which pins to light up
void binaryOn(int num) {
   turnAllOff();
   if(num >= 16) {
     // lights up the 5th bit
     analogWrite(rPin1, outVoltage);
     num -= 16;
   }
   if(num >= 8) {
     // lights up the 4th bit
      analogWrite(yPin1, outVoltage);
      num -= 8; 
   }
   if(num >= 4) {
      // lights up the 3rd bit
      analogWrite(gPin, outVoltage);
      num -= 4; 
   }
   if(num >= 2) {
      // lights up the 2nd bit
      analogWrite(yPin2, outVoltage);
      num -= 2;
   }
   if(num >= 1) {
      // lights up the 1st bit
      analogWrite(rPin2, outVoltage);
      num -= 1; 
   }
}
