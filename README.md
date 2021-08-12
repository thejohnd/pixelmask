# pixelmask
 An Arduino-powered sound-reactive masquerade mask

### New Years Pixel Mask
 _12-31-2014_

#### Hardware config:
  - Runs 2 strings in parallel, one of 4 pixels and one of 5, with the 5th in the center top
  
#### Software requirements:
 - `Adafruit NeoPixel library`
 
#### Connections *from* board *to* component pin:
 - `5V` to mic amp +
 - `GND` to mic amp -
 - `Analog pin` to microphone output
 - `Digital pin` to LED data input

#### Config:
 - Set `MIC_PIN` and `LED_PIN` vars
 - Set `N_PIXELS` to number of LEDs

>> *NOTES*
>>  - My mask uses 2 LED circuits in parallel to get a mirrored effect, if you're doing this, set `N_PIXELS` to the number of LEDs in your circuit (ie half of the total). This code will work without changes for a single LED circuit too.
>>  - Program uses bytes in for() loops, will cause problems if N_PIXELS > 256
>>  - Optional - reset `DC_OFFSET` to 0
