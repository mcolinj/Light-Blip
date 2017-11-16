# Light-Blip

Drop this repo into your Arduino/libraries directory and you should be able to use it
for lights development.

Blip is a single colored light that runs along a string from end to end.   By
including a variable sleep time, different velocities can be simulated.

Because the Blip is aware of the underlying colors it can move over different
colored backgrounds without doing too much damage to them.   Note that, if
the background is actively changing, then the Blip might leave a bit of a mark.

Download your fork of the repo as a .zip and then use

Sketch -> Include Library -> Add .ZIP Library...

to make the library and examples available to you in the Arduino IDE.
