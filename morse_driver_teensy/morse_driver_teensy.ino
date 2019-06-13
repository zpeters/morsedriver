#include <Bounce2.h>

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
boolean modalEnabled = false;

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

/** Modal functions **/
void toggleModal() {
  if (modalEnabled == true) {
    modalEnabled = false;
  } else {
    modalEnabled = true;
  }
}

/** Morse Processing **/
void processMorse(String myString) {
  // If we are switching modes we need to switch
  // and return to the event loop
  if (myString == "-..---") {
    Serial.println("Switching modes...");
    toggleModal();
    return;
  }

  // We are not switching modes so process
  // the input
  if (modalEnabled) {
    if (myString == ".-") {
      Serial.println("<up>");
    } else if (myString == "-.") {
      Serial.println("<down>");
    } else if (myString == "..") {
      Serial.println("<right>");
    } else if (myString == "--") {
      Serial.println("<left>");
    } else {
      Serial.println("<UNKNOWN>");
    }
  } else {
      Keyboard.print(myString);
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
