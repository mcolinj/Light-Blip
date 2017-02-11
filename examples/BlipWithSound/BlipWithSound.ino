#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

class Blip1076 {
  public:
      //
      //   Associate the blip with the NeoPixel controller and draw
      //   it at the starting location.
      //
      Blip1076(Adafruit_NeoPixel &pixels, uint32_t color, int x, int v); 

      int direction(void);
      void begin(void);
      
      /*
       *  Figure out how long to sleep to give the illusion of velocity
       */
      uint32_t sleep_time(void);
      /*
       * Return true if we are at the end of the string.  (need to reverse)
       */
      boolean at_string_end(void);
      //
      //  Move the blip according to its velocity, erase its previous spot, and
      //  Reverse the direction if we've gotten to the end.
      //
      void move(void);
      void speedup(float factor);
      
  private:
    Adafruit_NeoPixel &_pixels;     // keep a reference to controller blip can request display
    int _v;                         // velocity (negative moves down, positive moves up)
    int _x;                         // the location on the blip
    int _last_x;                    // where were we last?  (for cleaning up behind us)
    uint32_t _color;
    uint32_t _last_color;          // what was the color of the pixel where we drew the blip?
};


/* Blip
 *  
 * This represents a "car" of lights that can run on a string.
 * It just runs back and forth.    The car always moves only 1
 * light at a time.   The velocity of the car is expressed as
 * different delay.
 */
#define SIGNUM(a) ((a < 0) ? -1 : ((a > 0) ? 1 : 0));
#define ABS(a) ((a >= 0) ? a : -a)
#define SPEED_SCALE 1000   /* (at speed 1000, sleeps 1ms, at speed 1 sleeps for 1sec) */

Blip1076::Blip1076(Adafruit_NeoPixel &pixels, uint32_t color, int x, int v) 
    : _pixels(pixels), _color(color), _x(x), _v(v), _last_x(-1), _last_color(0) { }

void Blip1076::begin(void) {
    _pixels.begin();
    
    if (_x >= _pixels.numPixels()) {
        _x = _pixels.numPixels()-1;  // make to start on the string  (0 base!)
    }
    if (_x < 0) {
        _x = 0;
    }
        
    // show the blip at the specified location
    _last_x = _x;                               // remember where we were
    _last_color = _pixels.getPixelColor(_x);    // remember the color
    _pixels.setPixelColor(_x, _color);
  
}

int Blip1076::direction(void) {
    return SIGNUM(_v);
}

/*
 *  Figure out how long to sleep to give the illusion of velocity
 */
uint32_t Blip1076::sleep_time(void) {
    uint32_t st = SPEED_SCALE / ABS(_v);
    return st;
}

/*
 * Return true if we are at the end of the string.  (need to reverse)
 */
boolean Blip1076::at_string_end(void) {
    return (((_last_x == 0) && (_v < 0)) || 
            ((_last_x == _pixels.numPixels()-1) && (_v > 0)));
}

//
//  Move the blip according to its velocity, erase its previous spot, and
//  Reverse the direction if we've gotten to the end.
//
void Blip1076::move(void) {
    if (at_string_end()) {
        _v = -_v;                       // reverse direction
    }
    _x = _x + SIGNUM(_v);                        // move to left or right accordingly
    
    _pixels.setPixelColor(_last_x, _last_color); // erase previous blip using saved color
    _last_color = _pixels.getPixelColor(_x);     // remember the color for clean erase next time
    _pixels.setPixelColor(_x, _color);
    _pixels.show();
    _last_x = _x;                 // remember where we drew blip for next time
}

void Blip1076::speedup(float factor)
{
    _v = _v * factor;
}

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN_PIXELS     6
#define PIN_GATE_IN    9
#define IRQ_GATE_IN    0   /* mapped to pin 2 */

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      61

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);
Blip1076 blip = Blip1076(pixels, pixels.Color(150,0,150), 0, 33);

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
  Serial.begin(115200);
  
  //
  // start out with light green lights
  //
  for (int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();

  blip.begin();
 

}

void loop() {
  // put your main code here, to run repeatedly:
  blip.move();
  if (blip.sleep_time() < 100) {
    blip.speedup(0.999);
  }
  delay(blip.sleep_time()+1);
}
