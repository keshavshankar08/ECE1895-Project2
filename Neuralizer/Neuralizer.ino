#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <EEPROM.h>
                                        
// Pin initializations
const int startButtonPin = 2;           // button to trigger start the game
const int popCloseButtonPin = 3;        // button to trigger open and close lid
const int solenoidPin = 4;              // solenoid to open and close lid
const int chargePotentiometerPin = A1;  // potentiometer to set charge value
const int flashSwitchPin = 5;           // button to trigger flash laser
const int laserPin = 6;                 // laser to flash
const int greenLEDPin = 7;              // LED for action success
const int redLEDPin = 8;                // LED for action fail
const int speakerPin = 9;               // speaker to announce commands
const int sdCardPin = 10;               // sd card to hold audio files

// Other declarations/initializations
TMRpcm tmrpcm; 
int potentiometerValue;
float roundTime;
int roundScore;

// Function to setup microcontroller
void setup() {
  // setup serial
  Serial.begin(9600);
  Serial.println("Starting device...");

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
  pinMode(flashSwitchPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
}

// Function to loop microcontroller
void loop() {
  if(digitalRead(startButtonPin) == HIGH){
    Serial.println("Starting game...");
    roundTime = 0.0;
    delay(1000);
    if(PopIt()){
      CorrectLEDResponse();
      if(ChargeIt()){
        CorrectLEDResponse();
        if(FlashIt()){
          CorrectLEDResponse();
          if(CloseIt()){
            CorrectLEDResponse();
            Serial.println("Your score: ");
            tmrpcm.play("YourScore.wav");
            roundScore = round(roundTime/1000);
            Serial.println(roundScore);
            ScoreSoundSelector(roundScore);
            delay(2000);
            int highScore;
            EEPROM.get(0, highScore);
            Serial.println(highScore);
            if(roundScore < highScore) {
              Serial.println("New high score!");
              tmrpcm.play("NewHighScore.wav");
              EEPROM.put(0, roundScore);
            }
            else {
              Serial.println("High score is: ");
              Serial.println(highScore);
              tmrpcm.play("HighScoreIs.wav");
              ScoreSoundSelector(highScore);
            }
          }
          else {
            IncorrectLEDResponse();
            tmrpcm.play("WillSmithMad.wav");
          }
        }
        else {
          IncorrectLEDResponse();
          tmrpcm.play("WillSmithMad.wav");
        }
      }
      else {
        IncorrectLEDResponse();
        tmrpcm.play("WillSmithMad.wav");
      }
    }
    else {
      IncorrectLEDResponse();
      tmrpcm.play("WillSmithMad.wav");
    }
  }
}

// Runs "Pop it" game move
bool PopIt() {
  Serial.println("Pop It!");
  tmrpcm.play("PopIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(popCloseButtonPin) == HIGH) {
      buttonPressed = true;
      roundTime += millis() - startTime;
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

// Runs "Charge it" game move
bool ChargeIt() {
  Serial.println("Charge It!");
  tmrpcm.play("ChargeIt.wav");
  unsigned long startTime = millis();
  bool potentiometerMoved = false;
  while (millis() - startTime < 2000) {
    if (analogRead(chargePotentiometerPin) > 25) {
      potentiometerMoved = true;
      potentiometerValue = analogRead(chargePotentiometerPin);
      roundTime += millis() - startTime;
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

// Runs "Flash it" game move
bool FlashIt() {
  Serial.println("Flash It!");
  tmrpcm.play("FlashIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(flashSwitchPin) == HIGH) {
      buttonPressed = true;
      roundTime += millis() - startTime;
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

// Runs "Close It" game move
bool CloseIt() {
  Serial.println("Close It!");
  tmrpcm.play("CloseIt.wav");
  unsigned long startTime = millis();
  bool buttonPressed = false;
  while (millis() - startTime < 2000) {
    if (digitalRead(popCloseButtonPin) == HIGH) {
      buttonPressed = true;
      roundTime += millis() - startTime;
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

// Flashes the laser
void FlashLaser(){
  digitalWrite(laserPin, HIGH);
  delay(2000);
  digitalWrite(laserPin, LOW);
}

// Activates incorrect response game feedback
void IncorrectLEDResponse() {
  digitalWrite(redLEDPin, HIGH);
  delay(1000);
  digitalWrite(redLEDPin, LOW);
}

// Activates correct response game feedback
void CorrectLEDResponse() {
  digitalWrite(greenLEDPin, HIGH);
  delay(1000);
  digitalWrite(greenLEDPin, LOW);
}

// Plays the sound for a specific score 1-10
void ScoreSoundSelector(int score) {
  switch(score) {
    case 1:
      tmrpcm.play("Score1.wav");
      break;
    case 2:
      tmrpcm.play("Score2.wav");
      break;
    case 3:
      tmrpcm.play("Score3.wav");
      break;
    case 4:
      tmrpcm.play("Score4.wav");
      break;
    case 5:
      tmrpcm.play("Score5.wav");
      break;
    case 6:
      tmrpcm.play("Score6.wav");
      break;
    case 7:
      tmrpcm.play("Score7.wav");
      break;
    case 8:
      tmrpcm.play("Score8.wav");
      break;
    case 9:
      tmrpcm.play("Score9.wav");
      break;
    case 10:
      tmrpcm.play("Score10.wav");
      break;
    default:
      tmrpcm.play("ScoreOver.wav");
      break;
  }
}