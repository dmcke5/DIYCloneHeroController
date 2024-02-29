#include <Joystick.h> //https://github.com/MHeironimus/ArduinoJoystickLibrary
#define baudrate 19200 //Baudrate for serial.
#include <FastLED.h>

//Define button pins
byte buttonCount = 9; //Number of buttons in use. Change length of buttonPins, lastButtonState and currentButtonState to match.
byte buttonPins[9] = {0,1,2,3,4,5,6,15,14}; //Array to store digital pins used for buttons. Length must be the same as buttonCount.

//Button state arrays
byte lastButtonState[9]; //Empty State array for buttons last sent state. Must be same length as buttonPins
byte currentButtonState[9]; //Empty State array for buttons. MMust be same length as buttonPins

//Whammy bar config
const int whammyBar = A0;
const int deadBand = 25;
const int whammyMin = 140; //Adjust for Minimum whammy bar endpoint
const int whammyMax = 250; //Adjust for Max whammy bar endpoint

//Main Joystick setup
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
buttonCount, 0,       // Button Count, Hat Switch Count
false, false, true,     // X Y Z
false, false, false,  // Rx, Ry, Rz
false, false,          // Rudder, Throttle
false, false, false); // Accelerator, Brake, Steering

//LED Settings
#define LED_PIN     16
#define NUM_LEDS    5
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  Joystick.setZAxisRange(0, 254);
  Joystick.begin(false); //Initialise joystick mode with auto sendState disabled.
  for(int i = 0; i < buttonCount; i++){ //Set all button pins as input pullup. Change to INPUT if using external resistors.
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  leds[0] = CRGB::Orange;
  leds[1] = CRGB::Blue;
  leds[2] = CRGB::Yellow;
  leds[3] = CRGB::Red;
  leds[4] = CRGB::Green;
  FastLED.show();
}

void loop() {
  buttonRead();
  joypadButtons();
  int temp = analogRead(whammyBar);
  if(temp > whammyMin + deadBand){
    temp = map(temp, whammyMin, whammyMax, 0, 254);
    Joystick.setZAxis(temp);
  } else {
    Joystick.setZAxis(0);
  }
  Joystick.sendState();
}

void buttonRead(){ //Read button inputs and set state arrays.
  for (int i = 0; i < buttonCount; i++){
    int input = !digitalRead(buttonPins[i]);
    if (input != lastButtonState[i]){
      lastButtonState[i] = input;
    }
  }
}
void joypadButtons(){ //Set joystick buttons for USB output
  for(int i = 0; i < buttonCount; i++){
    if(lastButtonState[i] != currentButtonState[i]){
      Joystick.setButton(i, lastButtonState[i]);
      currentButtonState[i] = lastButtonState[i];
    }
  }
}
