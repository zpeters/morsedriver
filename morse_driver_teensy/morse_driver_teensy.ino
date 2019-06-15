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
unsigned long letterTimeout = 0;
unsigned long lastUpdate = 0;
boolean letterTimeoutBool = false;
String myLetter = "";
enum State {
  RAW,
  ASCII,
  COMMAND
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
    case COMMAND:
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
      commandState = COMMAND;
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
    case COMMAND:
      Serial.print("State: [COMMAND]\n");
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
    case COMMAND:
      editMode(myString);
      break;
    case RAW:
      Keyboard.print(myString);
      Keyboard.print(space);
      break;
    case ASCII:
      asciiMode(myString);
      break;
    default:
      Serial.println(myString);
      Keyboard.print(space);
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
  } else if (input == "-.-.") {
    output = "C";
  } else if (input == "-..") {
    output = "D";
  } else if (input == ".") {
    output = "E";
  } else if (input == "..-.") {
    output = "F";
  } else if (input == "--.") {
    output = "G";
  } else if (input == "....") {
    output = "H";
  } else if (input == "..") {
    output = "I";
  } else if (input == ".---") {
    output = "J";
  } else if (input == "-.-") {
    output = "K";
  } else if (input == ".-..") {
    output = "L";
  } else if (input == "--") {
    output = "M";
  } else if (input == "-.") {
    output = "N";
  } else if (input == "---") {
    output = "O";
  } else if (input == ".--.") {
    output = "P";
  } else if (input == "--.-") {
    output = "Q";
  } else if (input == ".-.") {
    output = "R";
  } else if (input == "...") {
    output = "S";
  } else if (input == "-") {
    output = "T";
  } else if (input == "..-") {
    output = "U";
  } else if (input == "...-") {
    output = "V";
  } else if (input == ".--") {
    output = "W";
  } else if (input == "-..-") {
    output = "X";
  } else if (input == "-.--") {
    output = "Y";
  } else if (input == "--..") {
    output = "Z";
  } else if (input == ".----") {
    output = "1";
  } else if (input == "..---") {
    output = "2";
  } else if (input == "...--") {
    output = "3";
  } else if (input == "....-") {
    output = "4";
  } else if (input == ".....") {
    output = "5";
  } else if (input == "-....") {
    output = "6";
  } else if (input == "--...") {
    output = "7";
  } else if (input == "---..") {
    output = "8";
  } else if (input == "----.") {
    output = "9";
  } else if (input == "-----") {
    output = "0";
  }
  Keyboard.print(output);
}

void editMode(String input) {
    if (input == ".-") {
      Serial.println("<up>");
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.release(KEY_UP_ARROW);
    } else if (input == "-.") {
      Serial.println("<down>");
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.release(KEY_DOWN_ARROW);
    } else if (input == "--") {
      Serial.println("<right>");
      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_RIGHT_ARROW);
    } else if (input == "..") {
      Serial.println("<left>");
      Keyboard.press(KEY_LEFT_ARROW);
      Keyboard.release(KEY_LEFT_ARROW);
    } else {
      Serial.println("<UNKNOWN>");
    }
}

/** Event Loop **/
void loop() {
     // Test if we have waited long enough for a space (aka a word is complete)
     // Continue processing the next input until we timeout
    if ((millis() - letterTimeout) > letterSpaceLength && letterTimeoutBool == true) {
       processMorse(myLetter);
       letterTimeoutBool = false;
       myLetter = "";
     }
    // If the button state has changed...
    if (pushbutton.update()) {
      if (((millis() - lastUpdate) > wordSpaceLength) && commandState != COMMAND) {
         Serial.println("ASCII OR RAW Word timeout");
         Keyboard.print(" ");
      }
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
          letterTimeout = millis();
          letterTimeoutBool = true;
        } else if (diff > ditLength && diff <= wordSpaceLength) {
          // if it was "dah" length then we got a dah add it to the letter accumulator
          myLetter += dah;
          letterTimeout = millis();
          letterTimeoutBool = true;
        }
      }
      lastUpdate = millis();
    }
}
