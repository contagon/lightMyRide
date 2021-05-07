#define FASTLED_INTERNAL
#include "FastLED.h"

#define OCTAVE 1 //   // Group buckets into octaves  (use the log output function LOG_OUT 1)
#define OCT_NORM 0 // Don't normalise octave intensities by number of bins
#define FHT_N 256 // set to 256 point fht
#include <FHT.h> // include the library


int noise[] = {204,195,100,90,85,80,75,75}; // noise for NANO

#define LED_PIN     5
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB


// TODO: figure out which way is width/height
// Params for width and height
const uint8_t kMatrixWidth = 11;
const uint8_t kMatrixHeight = 27;
// #define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define NUM_LEDS    15

CRGB leds[NUM_LEDS];

void setup() { 
    delay(1000);  

    // Setup printing
    Serial.begin(115200);
    Serial.print("Starting...");

    // Setup LEDS
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness (200);
    fill_solid(leds, NUM_LEDS, CRGB::Black); 
    FastLED.show();  

    // Setup reading from mic on adc port
    // TIMSK0 = 0; // turn off timer0 for lower jitter
    ADCSRA = 0xe5; // set the adc to free running mode
    ADMUX = 0x40; // use adc0
    DIDR0 = 0x01; // turn off the digital input for adc0
}



void loop() { 
    while (1) { // reduces jitter

        // TODO Figure out what this does
        cli();  // UDRE interrupt slows this way down on arduino1.0
     
        // Read 256 samples from microphone
        for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
            while (!(ADCSRA & 0x10)); // wait for adc to be ready
            ADCSRA = 0xf5; // restart adc
            byte m = ADCL; // fetch adc data
            byte j = ADCH;
            int k = (j << 8) | m; // form into an int
            k -= 0x0200; // form into a signed int
            k <<= 6; // form into a 16b signed int
            fht_input[i] = k; // put real data into bins
        }
        // FFT the mic data - saved in fht_oct_out[i]
        fht_window(); // window the data for better frequency response
        fht_reorder(); // reorder the data before doing the fht
        fht_run(); // process the data in the fht
        fht_mag_octave(); // take the output of the fht  fht_mag_log()

        // TODO Figure out what this does
        sei();
 

        // TODO Do logic to process FFT data into brightness
        // TODO Figure out interpolating between octaves
        // TODOo Figure out color control using potentiometer

        // goes through each octave. skip the first 1, which is not useful
        Serial.print("Printing FFT data");
        for (int i = 1; i < 8; i++) { 
            Serial.print(i); 
        }
    }
}
