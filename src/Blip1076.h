/* simple moving Blip with variable speed */ 
#ifndef __BLIP1076_H__
#define __BLIP1076_H__

/* Forward declaration to avoid dependency. */
class Adafruit_NeoPixel;

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

#endif   /* BLIP1076 */

