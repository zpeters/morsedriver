#include <Bounce2.h>

/*
 * TODO
 * 
 * Right now we are just printing back letters, instead we need to return back through functions and accumulate
 */

/*** Settings ****************************************/
// Assign buttons and leds
const int buttonPin = 0;
const int ledPin = 13;  // built-in led

// Assign morse timeouts
const int ditLength = 250;
const int dahLength = ditLength * 3;
const int letterSpaceLength = ditLength * 3;
const int wordSpaceLength = ditLength * 7;

// Assign morse symbols
String dit = ".";
String dah = "-";
String space = "/";

/*** Global State ****************************************/
Bounce pushbutton = Bounce();
unsigned long downAt = 0;
unsigned long upAt = 0;
unsigned long timeout = 0;
boolean timeoutBool = false;
String myLetter = "";
enum State {
  RAW,
  ASCII,
  EDIT
};
enum State commandState = RAW;

/*** Setup ****************************************/
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);
  pushbutton.attach(buttonPin);
  pushbutton.interval(5);
  pinMode(ledPin, OUTPUT);
  Serial.begin(38400);
  blinkOK();
}

/*** Blink Routines ****************************/
void blinkOK() {
  blinkDah();
  blinkDit();
  blinkDah();
}

void blinkDah() {
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
}

void blinkDit() {
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
}

/** State functions **/
void toggleState() {
  switch(commandState) {
    case EDIT:
      commandState = RAW;
      Serial.print("Switching to ");
      printState();
      break;
    case RAW:
      commandState = ASCII;
      Serial.print("Switching to ");
      printState();
      break;
    case ASCII:
      commandState = EDIT;
      Serial.print("Switching to ");
      printState();
      break;
    default:
      commandState = RAW;
      Serial.print("Switching to ");
      printState();
      break;
  }
}

void printState() {
  String stateText = "";
  switch(commandState) {
    case EDIT:
      Serial.print("State: [EDIT]\n");
      break;
    case RAW:
      Serial.print("State: [RAW]\n");
      break;
    case ASCII:
      Serial.print("State: [ASCII]\n");
      break;
  }
}

/** Morse Processing **/
void processMorse(String myString) {
  // If we are switching modes we need to switch
  // and return to the event loop
  if (myString == "-..---") {
    toggleState();
    return;
  }
  
  // Figure out which mode we are in and process
  switch(commandState) {
    case EDIT:
      editMode(myString);
      break;
    case RAW:
      Keyboard.print(myString);
      break;
    case ASCII:
      asciiMode(myString);
      break;
    default:
      Serial.println(myString);
      break;
  }
}

/** Mode Functions **/
void asciiMode(String input) {
  String output = "";

  if (input == ".-") {
    output = "A";
  } else if (input == "-...") {
    output = "B";
  }
 
  Keyboard.print(output);
}

void editMode(String input) {
    if (input == ".-") {
      Serial.println("<up>");
    } else if (input == "-.") {
      Serial.println("<down>");
    } else if (input == "..") {
      Serial.println("<right>");
    } else if (input == "--") {
      Serial.println("<left>");
    } else {
      Serial.println("<UNKNOWN>");
    }
}

/** Event Loop **/
void loop() {
     // Test if we have waited long enough for a space (aka a word is complete)
     // Continue processing the next input until we timeout
    if ((millis() - timeout) > letterSpaceLength && timeoutBool == true) {
       processMorse(myLetter);
       timeoutBool = false;
       myLetter = "";
       // send a space
       Keyboard.print(space);
     }
    // If the button state has changed...
    if (pushbutton.update()) {
      // Check if it has been pushed down...
      if (pushbutton.fallingEdge()) {
        digitalWrite(ledPin, HIGH);
        downAt = millis();
      // ...Or it is coming up
      } else if (pushbutton.risingEdge()) {
        digitalWrite(ledPin, LOW);
        upAt = millis();
        // if the button has been released, figure out how long it was held down for
        unsigned long diff = upAt - downAt;
        if (diff <= ditLength) {
          // if it was "dit" length then we got a dit add it to the letter accumulator
          myLetter += dit;
          timeout = millis();
          timeoutBool = true;
        } else if (diff > ditLength) {
          // if it was "dah" length then we got a dah add it to the letter accumulator
          myLetter += dah;
          timeout = millis();
          timeoutBool = true;
        }
      }
    }
}
