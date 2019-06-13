// http://www.pjrc.com/teensy/td_libs_Bounce.html
#include <Bounce.h>

const int buttonPin = 0;
const int ledPin = 13;
const int ditLength = 300;
const int dahLength = ditLength * 3;
const int letterSpaceLength = ditLength * 3;
const int wordSpaceLength = ditLength * 7;
Bounce pushbutton = Bounce(buttonPin, 10);
unsigned long downAt = 0;
unsigned long upAt = 0;
unsigned long timeout = 0;
boolean timeoutBool = false;
String myLetter = "";

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(38400);
  Serial.println("READY - OK");
  blinkOK();
}

void blinkOK() {
  // Dah
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dah
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
}

void blinkQuestion() {
    // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
    // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dah
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dah
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
 
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
}

void blinkUnderstood() {
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dah
  digitalWrite(ledPin, HIGH);
  delay(dahLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
  
  // Dit
  digitalWrite(ledPin, HIGH);
  delay(ditLength);
  digitalWrite(ledPin, LOW);
  delay(ditLength);
}

void processMorse(String myString) {
  Serial.print("Checking '");
  Serial.print(myString);
  Serial.print("' - ");
  if (myString == ".-") {
    Serial.println("'A'");
    Keyboard.write('a');
  } else if (myString == "-...") {
    Serial.println("Got 'B' sending Alt+Left Arrow");
    //Keyboard.set_modifier(MODIFIERKEY_ALT);
    //Keyboard.set_key1(KEY_LEFT);
    Keyboard.set_key1(KEY_BACKSPACE);
    Keyboard.send_now();
    delay(500);
    //Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();  
  } else if (myString == "--..") {
    Serial.println("'Z'");
    Keyboard.write('z');
  } else if (myString == ".---") {
      Serial.println("'J'");
      Keyboard.write('j');
  } else if (myString == "-.-") {
      Serial.println("'K'");
      Keyboard.write('k');
  } else if (myString == "..--..") {
      Serial.println("'?'");
      Keyboard.write('?');
  } else {
    Serial.println("UNKNOWN");
    //blinkQuestion();
  }
}

void loop() {
      // test how long we've been waiting for a space
    if ((millis() - timeout) > letterSpaceLength && timeoutBool == true) {
       //Serial.println("Timeout");
       processMorse(myLetter);
       timeoutBool = false;
       myLetter = "";
     }
  
  if (pushbutton.update()) {

 
    if (pushbutton.fallingEdge()) {
      // button pushed down  
      downAt = millis();
    } else if (pushbutton.risingEdge()) {
      // button released
      upAt = millis();
      unsigned long diff = upAt - downAt;
      if (diff <= ditLength) {
        // we got a dit
        myLetter += ".";
        //Serial.print(".");
        timeout = millis();
        timeoutBool = true;
      } else if (diff > ditLength) {
        // we got a dah
        myLetter += "-";
        //Serial.print("-");
        timeout = millis();
        timeoutBool = true;
      }
    }
  }
}
