// Import libraries
#define FASTLED_INTERNAL
#include "FastLED.h"
#include <Encoder.h>

// ALL PINS
#define PIN_BUTT1   4
#define PIN_BUTT2   4
#define PIN_ENC_1_A 2
#define PIN_ENC_1_B 2
#define PIN_ENC_2_A 2
#define PIN_ENC_2_B 2
#define PIN_DASH    5
#define PIN_F_SEAT  5
#define PIN_F_DOOR  5
#define PIN_B_DOOR  7

// LED Quantities
#define NUM_DASH    5
#define NUM_F_SEAT  5
#define NUM_F_DOOR  5
#define NUM_B_DOOR  7

// LED Definitions
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB dash[NUM_DASH];
CRGB f_seat[NUM_F_SEAT];
CRGB f_door[NUM_F_DOOR];
CRGB b_door[NUM_B_DOOR];

// Setup encoders
Encoder enc1(PIN_ENC_1_A, PIN_ENC_1_B);
Encoder enc2(PIN_ENC_2_A, PIN_ENC_2_B);

// Use these for current encoder/button behavior
long enc1Pos  = -999;
long enc2Pos  = -999;
bool butt1Pressed = false;
bool butt2Pressed = false;

unsigned long lastButt1 = 0;
unsigned long lastButt2 = 0;


// Setup everything
void setup() { 
    delay(1000);  

    // Setup printing
    Serial.begin(115200);
    Serial.print("Starting...");

    // Setup LEDS
    FastLED.addLeds<LED_TYPE, PIN_DASH, COLOR_ORDER>(dash, NUM_DASH).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_F_SEAT, COLOR_ORDER>(f_seat, NUM_F_SEAT).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_F_DOOR, COLOR_ORDER>(f_door, NUM_F_DOOR).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_B_DOOR, COLOR_ORDER>(b_door, NUM_B_DOOR).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness (200);
    fill_solid(dash, NUM_DASH, CRGB::Black); 
    fill_solid(f_seat, NUM_F_SEAT, CRGB::Black); 
    fill_solid(f_door, NUM_F_DOOR, CRGB::Black); 
    fill_solid(b_door, NUM_B_DOOR, CRGB::Black); 
    FastLED.show();  

    // Setup Encoder / button
    pinMode(PIN_BUTT1, INPUT_PULLUP);
    pinMode(PIN_BUTT2, INPUT_PULLUP);

    // TODO: Cool turn on sequence?
}

void updateSensors(){
    //  check encoder locations
    long newPosition = enc1.read();
    if (newPosition != enc1Pos) {
        Serial.println(newPosition);
        enc1Pos = newPosition;
    }
    newPosition = enc2.read();
    if (newPosition != enc2Pos) {
        Serial.println(newPosition);
        enc2Pos = newPosition;
    }

    // Check button click
    int btnState = digitalRead(PIN_BUTT1);
    if (btnState == LOW) {
        if (millis() - lastButt1 > 50) {
            Serial.println("Button pressed!");
            butt1Pressed = true;
        }
        else {
            butt1Pressed = false;
        }
        lastButt1 = millis();
    }
    btnState = digitalRead(PIN_BUTT2);
    if (btnState == LOW) {
        if (millis() - lastButt2 > 50) {
            butt2Pressed = true;
        }
        else {
            butt2Pressed = false;
        }
        lastButt2 = millis();
    }
}

void loop() {
    // Updates the following variables for in enabling LEDS
    // enc1Pos
    // enc2Pos
    // butt1Pressed
    // butt2Pressed
    updateSensors();

    int hue = enc1Pos % 255;
    int val = enc2Pos % 255;
    fill_solid( dash,   NUM_DASH,   CRGB( hue, 255, val) );
    fill_solid( f_seat, NUM_F_SEAT, CRGB( hue, 255, val) );
    fill_solid( f_door, NUM_F_DOOR, CRGB( hue, 255, val) );
    fill_solid( b_door, NUM_B_DOOR, CRGB( hue, 255, val) );

    FastLED.show();
}
