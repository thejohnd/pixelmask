/* New Years Pixel Mask

>> Hardware config:
  - Runs 2 strings in parallel, one of 4 pixels and one of 5, with the 5th in the center top
  
  **program uses bytes in for() loops, will cause problems if N_PIXELS > 256
 
Software requirements:
 - Adafruit NeoPixel library
 
Connections:
 - 5 V to mic amp +
 - GND to mic amp -
 - Analog pinto microphone output (configurable below)
 - Digital pin to LED data input (configurable below)
 
Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/
#include <Adafruit_NeoPixel.h>
 
#define N_PIXELS   5  // Number of pixels you are using
#define MIC_PIN    1  // Microphone is attached to Trinket GPIO #2/Gemma D2 (A1)
#define LED_PIN    0  // NeoPixel LED strand is connected to GPIO #0 / D0
#define DC_OFFSET  1.65  // DC offset in mic signal - if unusure, leave 0
#define NOISE      100  // Noise/hum/interference in mic signal
#define SAMPLES    60  // Length of buffer for dynamic level adjustment
#define TOP        (N_PIXELS +1) // Allow dot to go slightly off scale
 
byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0,      // Frame counter for storing past volume data
  y = 0;              //counting up to chase
  
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,     // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg = 512,
  height = 1;
  
  
uint8_t 
     bright = 10;
         
 
Adafruit_NeoPixel  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  memset(vol, 0, sizeof(vol));
  strip.begin();
}
void loop() {
  uint16_t minLvl, maxLvl;
  int   n;
  n   = analogRead(MIC_PIN);                 // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET);            // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);      // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)
  
  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);
 
  if(height < 0L)       height = 0;      // Clip output
  else if(height > TOP) height = TOP;
  if(height > peak)     peak = height; // Keep 'peak' dot at top
 
  bright = map(maxLvlAvg, 0, 512, 15, 120); //adjust brightness based on sound levels. edit last two params for min/max output
  
  if (height < (N_PIXELS*0.9)){
  for(int i=0; i<N_PIXELS; i++) {  
    if(i >= height){               
       strip.setPixelColor(i,  0, (maxLvlAvg/10), 20);
       
    } 
    else strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
    
    }
  }
  else if (y>=8){
    theaterChaseRainbow(50);
    y=0;
  }
  else {
         for (int i=0; i<N_PIXELS; i++){
         //strip.setPixelColor(i,Wheel(map(i,0,strip.numPixels()-1,30,150)));
         strip.setPixelColor(i,  bright*(0.75*((i+1)*0.1)), bright*(0.75*((i+1)*0.1)), bright*0.75*(i+1));
         strip.setBrightness(bright+15);
         strip.show();
         delay(20);
       }
       y++;
       /*for(int i=0; i<N_PIXELS; i++) {  
         if(i >= height){               
           strip.setPixelColor(i,  0, 7, 20);
         }
       } */
       //strip.show();
  }
  strip.setBrightness(bright);
  strip.show(); // Update strip
 
  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter
 
  // Get volume range of prior frames
  minLvl = maxLvl = vol[0];
  for(int i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else if(vol[i] > maxLvl) maxLvl = vol[i];
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg = (maxLvlAvg * 63 + maxLvl) >> 6; // (fake rolling average)
}
 
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
/*uint32_t Wheel(byte WheelPos) {
  WheelPos= WheelPos+r; 
  if(bright < 85) {
   return strip.Color(WheelPos -bright, bright - WheelPos * 3, 255);
  } else if(bright < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(height, height, height);
  }
}*/
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
if(WheelPos < 85) {
return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 200);
} else if(WheelPos < 170) {
WheelPos -= 85;
return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else {
WheelPos -= 170;
return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (byte j=0; j < 3; j++) {     // cycle all 256 colors in the wheel
    for (int i=0; i < N_PIXELS; i++) {
        strip.setPixelColor(i, Wheel( (i+j) % 255));    //turn pixels on in sequence
        strip.setBrightness(bright+25);
        strip.setPixelColor(i-1, 0);                               //turn previous pixel off
        
        strip.show();
       
        delay(wait);
       
        }
    
  }
}
