# pixelmask
 An Arduino-powered sound-reactive masquerade mask

 New Years Pixel Mask
 12-31-14

>> Hardware config:
  - Runs 2 strings in parallel, one of 4 pixels and one of 5, with the 5th in the center top
  
  **program uses bytes in for() loops, will cause problems if N_PIXELS > 256
 
Software requirements:
 - Adafruit NeoPixel library
 
Connections:
 - 5 V to mic amp +
 - GND to mic amp -
 - Analog pinto microphone output
 - Digital pin to LED data input