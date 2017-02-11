/*
 *  Simple Blip test.
 *
 */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Blip1076.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Blip1076 blip = Blip1076(pixels, pixels.Color(150,0,150), 0, 1);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  blip.begin(); // This initializes the NeoPixel library.
}

//
// Meter converts a number N into a number of illuminated lights.
//
// Display the meter value by transitioning the lights from from_value
// to the to_value.   Assumes that from_value lights are already set.
//
#define METER_SPEED 1
void meter_from(int from_value, int to_value) {
  // move from low to high
  if (from_value <= to_value) {
      for (int i=from_value; i<=to_value; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 50, 0));

          pixels.show();

          delay(METER_SPEED);
      }
  } else {
      for (int i=from_value; i>=to_value; i--) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));

          pixels.show();

          delay(METER_SPEED); 
      }
  }
}

void loop() {
    blip.move();
    delay(blip.speed_as_delay());
}
