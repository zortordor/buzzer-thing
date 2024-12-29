#include "pitches.h"

// Define the connections to the Arduino
const int pinCLK = 2; // Connected to CLK on the rotary encoder
const int pinDT = 3;  // Connected to DT on the rotary encoder
const int pinSW = 4;  // Connected to SW on the rotary encoder

//Define the 7-segment-display connections
const int pinA = 22;
const int pinB = 23;
const int pinC = 24;
const int pinD = 25;
const int pinE = 26;
const int pinF = 27;
const int pinG = 28;

//Define other connections
const int buzzerPin = 8;
const int redLED = 11;
const int blueLED = 10;
const int buttonRight = 6;
const int buttonLeft = 7;

// Variables to hold the current and last encoder position
volatile int encoderPos = 0;
int lastEncoderPos = 0;

// Variables to keep track of the state of the pins
int lastCLK;
int currentCLK;

//Define notes
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// Function to display a number on a single digit
void turnOnElem(const int segments[]) {
  digitalWrite(pinA, segments[0] ? HIGH : LOW);
  digitalWrite(pinB, segments[1] ? HIGH : LOW);
  digitalWrite(pinC, segments[2] ? HIGH : LOW);
  digitalWrite(pinD, segments[3] ? HIGH : LOW);
  digitalWrite(pinE, segments[4] ? HIGH : LOW);
  digitalWrite(pinF, segments[5] ? HIGH : LOW);
  digitalWrite(pinG, segments[6] ? HIGH : LOW);
}

// Function to map a number (0-9) to its 7-segment representation
void convertToArray(int number, int segments[7]) {
  const int digitMap[10][7] = {
      {1, 1, 1, 1, 1, 1, 0}, // 0
      {0, 1, 1, 0, 0, 0, 0}, // 1
      {1, 1, 0, 1, 1, 0, 1}, // 2
      {1, 1, 1, 1, 0, 0, 1}, // 3
      {0, 1, 1, 0, 0, 1, 1}, // 4
      {1, 0, 1, 1, 0, 1, 1}, // 5
      {1, 0, 1, 1, 1, 1, 1}, // 6
      {1, 1, 1, 0, 0, 0, 0}, // 7
      {1, 1, 1, 1, 1, 1, 1}, // 8
      {1, 1, 1, 1, 0, 1, 1}  // 9
  };
  for (int i = 0; i < 7; i++) {
    segments[i] = digitMap[number][i];
  }
}

//Function to play a note depending on the rotary encoder position and which button is pressed
static playNote(int position, char button) {
  if (position == 0){
    if (button == "RIGHT"){tone(buzzerPin, 500);} else {tone(buzzerPin, 525);}
  }
  if (position == 1){
    if (button == "RIGHT"){tone(buzzerPin, 550);} else {tone(buzzerPin, 575);}
  }
  if (position == 2){
    if (button == "RIGHT"){tone(buzzerPin, 600);} else {tone(buzzerPin, 625);}
  }
  if (position == 3){
    if (button == "RIGHT"){tone(buzzerPin, 650);} else {tone(buzzerPin, 675);}
  }
  if (position == 4){
    if (button == "RIGHT"){tone(buzzerPin, 700);} else {tone(buzzerPin, 725);}
  }
  if (position == 5){
    if (button == "RIGHT"){tone(buzzerPin, 750);} else {tone(buzzerPin, 775);}
  }
  if (position == 6){
    if (button == "RIGHT"){tone(buzzerPin, 800);} else {tone(buzzerPin, 825);}
  }
  if (position == 7){
    if (button == "RIGHT"){tone(buzzerPin, 850);} else {tone(buzzerPin, 875);}
  }
  if (position == 8){
    if (button == "RIGHT"){tone(buzzerPin, 900);} else {tone(buzzerPin, 925);}
  }
  if (position == 9){
    if (button == "RIGHT"){tone(buzzerPin, 950);} else {tone(buzzerPin, 1000);}
  }
  delay(100);
  noTone(buzzerPin);
}

// Setup function runs once at the start of the program
void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);

  int segments[7];
  convertToArray(0, segments);
  turnOnElem(segments);

  pinMode(pinCLK, INPUT_PULLUP);
  pinMode(pinDT, INPUT_PULLUP);
  pinMode(pinSW, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);

  // Attach an interrupt to the CLK pin
  attachInterrupt(digitalPinToInterrupt(pinCLK), readEncoder, CHANGE);

  // Initialize the serial communication
  Serial.begin(9600);

  // Read the initial state of CLK
  lastCLK = digitalRead(pinCLK);
}

// Main program loop
void loop() {
  // Check if the encoder has moved
  if (encoderPos != lastEncoderPos) {
    Serial.print("Position: ");
    Serial.println(encoderPos);
    lastEncoderPos = encoderPos;
  }

  // Check if the button is pressed
  if (digitalRead(pinSW) == LOW) {
    // Reset the encoder position to 0
    encoderPos = 0;
    Serial.println("Reset to 0");
    int segments[7];
    convertToArray(encoderPos, segments);
    turnOnElem(segments);
    // Wait for the button to be released
    while (digitalRead(pinSW) == LOW) delay(10);
  }

  if (digitalRead(buttonRight) == LOW) {
    digitalWrite(redLED, HIGH);
    playNote(encoderPos, "RIGHT");
  } else {digitalWrite(redLED, LOW);}
  
  if (digitalRead(buttonLeft) == LOW) {
    digitalWrite(blueLED, HIGH);
    playNote(encoderPos, "LEFT");
  } else {digitalWrite(blueLED, LOW);}
}

// Interrupt service routine for reading the encoder
void readEncoder() {
  
  currentCLK = digitalRead(pinCLK);
  // If the current state of CLK is different from the last state
  // then a pulse occurred
  if (currentCLK != lastCLK) {
    int segments[7];
    // If the DT state is different from the CLK state
    // then the encoder is rotating clockwise
    if (digitalRead(pinDT) != currentCLK) {
      encoderPos++;
      if (encoderPos > 9){ 
        encoderPos = 0;
        Serial.println("Reset to 0");
      }
      convertToArray(encoderPos, segments);
      turnOnElem(segments);
    } else {
      // Otherwise, it's rotating counterclockwise
      encoderPos--;
      if (encoderPos < 0){ 
        encoderPos = 0;
        Serial.println("Reset to 0");
      }
      convertToArray(encoderPos, segments);
      turnOnElem(segments);
    }
  }
  // Update lastCLK with the current state for the next pulse detection
  lastCLK = currentCLK;
}