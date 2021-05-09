// Import libraries
#define FASTLED_INTERNAL
#include "FastLED.h"
#include <Encoder.h>

// ALL PINS
#define PIN_ENC_1_A 3
#define PIN_ENC_1_B 2
#define PIN_BUTT1   4

#define PIN_ENC_2_A 6
#define PIN_ENC_2_B 5
#define PIN_BUTT2   7

#define PIN_DASH    8
#define PIN_SEAT    9
#define PIN_F_DOOR  10
#define PIN_B_DOOR  11

// LED Quantities
#define NUM_DASH    29
#define NUM_SEAT    21
#define NUM_F_DOOR  21
#define NUM_B_DOOR  21

// LED Definitions
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB dash[NUM_DASH];
CRGB seat[NUM_SEAT];
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

// Starting color
#define START_COLOR 15
int lastHue = -1;
int lastVal = -1;
bool rainbowMode = false;

// Setup everything
void setup() { 
    delay(1000);  

    // Setup printing
    Serial.begin(115200);
    Serial.print("Starting...");

    // Setup LEDS
    FastLED.addLeds<LED_TYPE, PIN_DASH, COLOR_ORDER>(dash, NUM_DASH).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_SEAT, COLOR_ORDER>(seat, NUM_SEAT).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_F_DOOR, COLOR_ORDER>(f_door, NUM_F_DOOR).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, PIN_B_DOOR, COLOR_ORDER>(b_door, NUM_B_DOOR).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness (200);
    fill_rainbow(dash,   NUM_DASH,   1);
    fill_rainbow(seat,   NUM_SEAT,   1); 
    fill_rainbow(f_door, NUM_F_DOOR, 1); 
    fill_rainbow(b_door, NUM_B_DOOR, 1); 
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
        Serial.print("Encoder 1: ");
        Serial.println(newPosition);
        enc1Pos = newPosition;
    }
    newPosition = enc2.read();
    if (newPosition != enc2Pos) {
        Serial.print("Encoder 2: ");
        Serial.println(newPosition);
        enc2Pos = newPosition;
    }

    // Check button click
    int btnState = digitalRead(PIN_BUTT1);
    if (btnState == LOW) {
        if (millis() - lastButt1 > 50) {
            Serial.println("Button 1 pressed!");
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
            Serial.println("Button 2 pressed!");
            butt2Pressed = true;
        }
        else {
            butt2Pressed = false;
        }
        lastButt2 = millis();
    }
}

void fill_slow(CRGB* leds, int num, CHSV new_color, int wait){
    for(int i=0;i<num;i++){
        leds[i] = new_color;
        FastLED.show();
        delay(wait);
    }
}

void fill_rainbow(CRGB* leds, int num, int wait) {
    for (int j = 0; j < 255; j=j+5) {
        for (int i = 0; i < num; i++) {
            leds[i] = CHSV(i - (j * 2), 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
            FastLED.show();
            delay(wait); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
        }
    }
}

void loop() {
    // Updates the following variables for in enabling LEDS
    // enc1Pos
    // enc2Pos
    // butt1Pressed
    // butt2Pressed
    updateSensors();

    if(butt1Pressed){
        rainbowMode = !rainbowMode;
    }

    if(rainbowMode){
        fill_rainbow(dash,   NUM_DASH,   1);
        fill_rainbow(seat,   NUM_SEAT,   1); 
        fill_rainbow(f_door, NUM_F_DOOR, 1); 
        fill_rainbow(b_door, NUM_B_DOOR, 1); 
    }
    else{
        int hue = enc1Pos % 255 + START_COLOR;
        int val = 255; //enc2Pos % 255;
        if(val < 0){
            val = 0;
        }
        else if(val > 255){
            val = 255;
        }
        if(hue != lastHue || val != lastVal){
            fill_slow( dash,   NUM_DASH,   CHSV(hue,255,val), 25 );
            fill_slow( seat,   NUM_SEAT, CHSV(hue,255,val), 25 );
            fill_slow( f_door, NUM_F_DOOR, CHSV(hue,255,val), 25 );
            fill_slow( b_door, NUM_B_DOOR, CHSV(hue,255,val), 25 );
            lastHue = hue;
            lastVal = val;
        }
    }

    FastLED.show();
}
