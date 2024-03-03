#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
                                        
// Pin initializations
const int startButtonPin = 2;           // button to trigger start the game
const int popCloseButtonPin = 3;        // button to trigger open and close lid
const int solenoidPin = 4;              // solenoid to open and close lid
const int chargePotentiometerPin = A1;  // potentiometer to set charge value
const int flashButtonPin = 5;           // button to trigger flash laser
const int laserPin = 6;                 // laser to flash
const int speakerPin = 9;               // speaker to announce commands
const int sdCardPin = 10;               // sd card to hold audio files

// Other declarations/initializations
TMRpcm tmrpcm; 
int potentiometerValue;

// Function to setup microcontroller
void setup() {
  Serial.begin(9600);

  // setup speaker
  tmrpcm.speakerPin = speakerPin;
  if (!SD.begin(sdCardPin)) {
    Serial.println("SD fail");  
    return;
  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.volume(7);

  // set pin modes
  pinMode(startButtonPin, INPUT);
  pinMode(popCloseButtonPin, INPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(chargePotentiometerPin, INPUT);
  pinMode(flashButtonPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
}

// Function to loop microcontroller
void loop() {
  if(digitalRead(startButtonPin) == HIGH){
    Serial.println("Starting game...");
    delay(1000);
    if(PopIt()){
      if(ChargeIt()){
        if(FlashIt()){
          if(CloseIt()){

          }
        }
      }
    }
  }
}

bool PopIt() {
  Serial.println("Pop It!");
  tmrpcm.play("PopIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(popCloseButtonPin) == HIGH) {
      buttonPressed = true;
      break;
    }
  }
  if (buttonPressed) {
    digitalWrite(solenoidPin, HIGH);
    Serial.println("You passed Pop It!");
    return true;
  } else {
    digitalWrite(solenoidPin, LOW);
    Serial.println("You failed Pop It!");
    return false;
  }
}

bool ChargeIt() {
  Serial.println("Charge It!");
  tmrpcm.play("ChargeIt.wav");
  unsigned long startTime = millis();
  bool potentiometerMoved = false;
  while (millis() - startTime < 2000) {
    if (analogRead(chargePotentiometerPin) > 25) {
      potentiometerMoved = true;
      potentiometerValue = analogRead(chargePotentiometerPin);
      break;
    }
  }
  if (potentiometerMoved) {
    Serial.println("You passed Charge It!");
    return true;
  } else {
    digitalWrite(solenoidPin, LOW);
    Serial.println("You failed Charge It!");
    return false;
  }
}

bool FlashIt() {
  Serial.println("Flash It!");
  tmrpcm.play("FlashIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(flashButtonPin) == HIGH) {
      buttonPressed = true;
      break;
    }
  }
  if (buttonPressed) {
    Serial.println("You passed Flash It!");
    FlashLaser();
    return true;
  } else {
    digitalWrite(laserPin, LOW);
    digitalWrite(solenoidPin, LOW);
    Serial.println("You failed Flash It!");
    return false;
  }
}

bool CloseIt() {
  Serial.println("Close It!");
  tmrpcm.play("CloseIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(popCloseButtonPin) == HIGH) {
      buttonPressed = true;
      break;
    }
  }
  if (buttonPressed) {
    digitalWrite(solenoidPin, LOW);
    Serial.println("You passed Close It!");
    return true;
  } else {
    digitalWrite(solenoidPin, LOW);
    Serial.println("You failed Close It!");
    return false;
  }
}

void FlashLaser(){
  digitalWrite(laserPin, HIGH);
  delay(3000);
  digitalWrite(laserPin, LOW);
}