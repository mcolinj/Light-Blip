#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Blip1076.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN_PIXELS     6
#define PIN_GATE_IN    9
#define IRQ_GATE_IN    0   /* mapped to pin 2 */

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      31

// Main loop has minimum delay to provide cap on velocity
#define MINIMUM_DELAY_PER_LOOP 25

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);
Blip1076 blip = Blip1076(pixels, pixels.Color(150,0,150), 0, 330);

int interrupt_count = 0;

//
//   This function gets installed as an "interrupt handler".
//   Every time the input changes according to the way the
//   handler is installed, this function gets called.
//
void soundISR()
{
  int pin_val;

  interrupt_count++;
  pin_val = digitalRead(PIN_GATE_IN);
  if (pin_val) {
     blip.speedup(1.2);
  }
}

void setup() {
  // configure input to interrupt
  pinMode(PIN_GATE_IN, INPUT);
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // put your setup code here, to run once:
  // Serial.begin(115200);    /* Use this for debugging */
  
  //
  // start out with light green lights
  //
  for (int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 60));
  }
  pixels.show();

  blip.begin();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  blip.move();

  //
  // once a certain speed is acheived, we want
  // a gradual decay
  if (blip.sleep_time() < 100) {
    blip.speedup(0.999);
  }

  /* 
   * We delay based on the sleep time to simulate
   * velocity.
   */
  delay(blip.sleep_time());

  /*
   * Put a limit on the maximum speed with
   * an additional delay.
   */
   delay(MINIMUM_DELAY_PER_LOOP);
}
