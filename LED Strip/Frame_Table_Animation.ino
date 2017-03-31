// Frame Table Animation
// Malcolm Knapp
// 3/30/17
// adapted from NeoAnim by Circuit Playground
// MIT License

#include <Adafruit_NeoPixel.h>
#include "Frame_Table.h" //this is the name of the animation derived from the neoAnim.png bitmap file

#define LED_STRIP_PIN 6
#define STRIP_LENGTH 32

long startRender;
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin (115200);
  strip.begin(); // Initialize NeoPixel object
  strip.show(); // Initialize all pixels to 'off'

  // Start looping animation
  playAnim(PixelData, FPS, sizeof(PixelData), true);
}

// Global values used by the animation and sound functions
uint16_t         *pixelBaseAddr, // Address of active animation table
                 pixelLen,      // Number of pixels in active table
                 pixelIdx;      // Index of first pixel of current frame
uint8_t           pixelFPS;     // Frames/second for active animation
boolean           pixelLoop;     // If true, animation repeats

// Begin playing a NeoPixel animation from a PROGMEM table
void playAnim(const uint16_t *addr, uint8_t fps, uint16_t bytes, boolean repeat) {
  pixelBaseAddr = (uint16_t *) addr;
  if (addr) {
    pixelFPS    = fps;
    pixelLen    = bytes / 2; //size() returns # of bytes so number of ints is bytes/2
    pixelLoop   = repeat; //if set to 'repeat' it'll loop, set to 0 to play once only
    pixelIdx    = 0;
  } else {
    for (uint8_t i = 0; i < STRIP_LENGTH; i++) strip.setPixelColor(i, 0, 0, 0); // clear strip
  }
}

uint32_t prev = 0; // Time of last NeoPixel refresh

void loop() {
  uint32_t t;      // Current time in milliseconds

  if (((t = millis()) - prev) > (1000 / pixelFPS)) { // trigger when frame needs updating
  // Show LEDs rendered on prior pass.  It's done this way so animation timing
  // is a bit more consistent (frame rendering time may vary slightly).
  strip.show();

  prev = t; // Save refresh time for next frame sync

  if (pixelBaseAddr) {
    startRender = micros();
    for (uint8_t i = 0; i < STRIP_LENGTH; i++) { // For each NeoPixel...
      // Read pixel color from PROGMEM table
      uint16_t rgb = pgm_read_word(&pixelBaseAddr[pixelIdx++]);
      
      // Expand 16-bit color to 24 bits using gamma tables
      // RRRRRGGGGGGBBBBB -> RRRRRRRR GGGGGGGG BBBBBBBB
      strip.setPixelColor(i,
                          pgm_read_byte(&gamma5[ rgb >> 11        ]),  // last 5 bits are all that is left at the shift
                          pgm_read_byte(&gamma6[(rgb >>  5) & 0x3F]),  // mask is 0B00000000000111111
                          pgm_read_byte(&gamma5[ rgb        & 0x1F])); // mask is 0B00000000000011111
    }
    Serial.print ("Render time: "); Serial.println(micros() - startRender); //send rendering time
    if (pixelIdx >= pixelLen) { // End of animation table reached
      if (pixelLoop) { // Repeat animation
        pixelIdx = 0; // Reset index to start of table
      } else {        // else switch off LEDs
        playAnim(NULL, FPS, 0, false);
      }
    }
  }

  }
}
