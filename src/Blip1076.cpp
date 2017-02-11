// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <AdaFruit_NeoPixel.h>
#include <Blip1076.h>

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

