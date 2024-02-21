#include <PCM.h>

const int openClosePin = 2;
const int laserPin = 3;
const int chargeItPin = A1;
const int solenoidPin = 4;
const int speakerPin = 5;
const int startButton = 6;
const int flashItPin = 7;

const unsigned char sample[] PROGMEM = {
  -61.68172,-59.93872,-63.46055,-64.28840,-67.38644,-74.74597,-90.30900,-74.74597,-69.48114,-67.38644,-68.03013,-65.70002,-66.22660,-62.00953,-63.86461,-61.06104,-63.07444,-60.48176,-63.86461,-60.20600,-63.86461,-61.68172,-63.07444,-66.78717,-69.48114,-78.26780,-76.32960,-74.74597,-76.32960,-65.20355,-66.22660,-61.36584,-61.68172,-59.42764,-58.26780,-57.24475,-56.68417,-57.43995,-57.63963,-57.43995,-57.84401,-57.63963,-57.43995,-58.94496,-58.94496,-61.36584,-62.00953,-65.20355,-74.74597,-80.76657,-66.22660,-66.78717,-61.36584,-59.93872,-58.05332,-56.68417,-55.66112,-54.18540,-52.92436,-51.92744,-51.51861,-50.93934,-50.66357,-50.66357,-50.39629,-50.57356,-50.30900,-50.22257,-50.57356,-50.66357,-50.75453,-50.30900,-50.75453,-50.57356,-51.12817,-51.12817,-51.51861,-51.61903,-51.82341,-52.24720,-52.80777,-53.65882,-53.16235,-54.18540,-53.40704,-54.32219,-54.60240,-54.18540,-54.05073,-54.32219,-54.46116,-54.18540,-54.60240,-55.04044,-55.34524,-55.19150,-55.04044,-54.89196,-54.46116,-53.91812
};

void setup() {
  Serial.begin(9600);
  pinMode(laserPin, OUTPUT);
  pinMode(chargeItPin, INPUT);
  pinMode(startButton, INPUT);
  pinMode(openClosePin, INPUT);
  startPlayback(sample, sizeof(sample));
}

void loop() {
  if(digitalRead(startButton) == HIGH){
    Serial.println("Starting game...");
    delay(1000);
    if(bopIt()){
      if(chargeIt()){
        
      }
    }
  }
}

bool bopIt() {
  Serial.println("Bop It!");
  unsigned long startTime = millis();
  bool buttonPressed = false;

  while (millis() - startTime < 2000) {
    if (digitalRead(openClosePin) == HIGH) {
      buttonPressed = true;
      break;
    }
  }

  if (buttonPressed) {
    Serial.println("You passed Bop It!");
    return true;
  } else {
    Serial.println("Failed! Try again.");
    return false;
  }
}

bool chargeIt() {
  Serial.println("Charge It!");
  unsigned long startTime = millis();
  bool potentiometerReached = false;

  while (millis() - startTime < 2000) {
    if (analogRead(chargeItPin) > 30) {
      potentiometerReached = true;
      break;
    }
  }

  if (potentiometerReached) {
    Serial.println("You passed Charge It!");
    return true;
  } else {
    Serial.println("Failed! Try again.");
    return false;
  }
}