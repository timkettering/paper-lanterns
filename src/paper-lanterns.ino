#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define DATA_PIN    D0
#define NUM_LEDS_16IN   9
#define NUM_LEDS_10IN   7

CRGB leds0[NUM_LEDS_10IN];
CRGB leds1[NUM_LEDS_16IN];
CRGB leds2[NUM_LEDS_16IN];
CRGB leds3[NUM_LEDS_10IN];
//CRGB leds4[NUM_LEDS];

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120

// using this delta gives the rainbow() fill a
// cool iridiscent effect for th `pearl` function
#define PEARL_RAINBOW_DELTA 57

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t lantern1Counter = 0;
uint8_t lantern2Counter = 0;

// rotating timers for diff speeds
uint8_t wallCounterSlow = 0;
uint8_t wallCounterFast = 0;

static uint8_t led0Bri = 255;
static uint8_t led1Bri = 255;
static uint8_t led2Bri = 255;
static uint8_t led3Bri = 255;

// various palettes
CRGBPalette16 AcidBreath_p = CRGBPalette16( CRGB::Black, CRGB::DarkGreen, CRGB::Green, CRGB::Yellow );
CRGBPalette16 FadeToBlack_p = CRGBPalette16( CRGB::White, CRGB::Black, CRGB::White, CRGB::Black );
CRGBPalette16 WarmWhites_p = CRGBPalette16( CHSV(35,200,255), CHSV(44,170,255), CHSV(44,170,255), CHSV(35,200,255) );

void setup() {

  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,EOrder::RGB,DATA_RATE_MHZ(10)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<NEOPIXEL,D0>(leds0, NUM_LEDS_10IN);
  FastLED.addLeds<NEOPIXEL,D1>(leds1, NUM_LEDS_16IN);
  FastLED.addLeds<NEOPIXEL,D2>(leds2, NUM_LEDS_16IN);
  FastLED.addLeds<NEOPIXEL,D3>(leds3, NUM_LEDS_10IN);
//  FastLED.addLeds<NEOPIXEL,D4>(leds4, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
//  lantern0();
//  lantern1();
//  lantern2();
  pearl();
//    comfortWhite();
//    operatingRoom();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
    wallCounterFast++;
    lantern1Counter = (lantern1Counter + 1) % NUM_LEDS_16IN;
    lantern2Counter++;

  } // slowly cycle the "base color" through the rainbow

    EVERY_N_MILLISECONDS(60) {

        // each specific function will toggle the random dimming/flaring
        if(true) {
            // each iteration, adjust brightness by a random number
            led0Bri = adjustBrightness(led0Bri);
            led1Bri = adjustBrightness(led1Bri);
            led2Bri = adjustBrightness(led2Bri);
            led3Bri = adjustBrightness(led3Bri);
        }
    }

  EVERY_N_MILLISECONDS(200) {
    wallCounterSlow++;

    // each specific function will toggle the random dimming/flaring
    if(true) {
        // each iteration, adjust brightness by a random number
        led0Bri = adjustBrightness(led0Bri);
        led1Bri = adjustBrightness(led1Bri);
        led2Bri = adjustBrightness(led2Bri);
        led3Bri = adjustBrightness(led3Bri);
    }
  }
}

void lantern0() {
    fill_rainbow(leds0, NUM_LEDS_10IN, gHue, 7);
}

void lantern1() {

    leds1[lantern1Counter] = CRGB::Black;
    leds1[lantern1Counter + 1] = CRGB::Red;


    leds1[lantern1Counter + 5] = CRGB::Black;
    leds1[lantern1Counter + 6] = CRGB::Yellow;
//    fill_solid(leds1, NUM_LEDS_16IN, CRGB::Blue);
}

void lantern2() {

    fill_solid(leds2, NUM_LEDS_16IN, CRGB(gHue, gHue, gHue));
}

void operatingRoom() {

  fill_solid( leds0, NUM_LEDS_10IN, CRGB::White);
  fill_solid( leds1, NUM_LEDS_16IN, CRGB::White);
  fill_solid( leds2, NUM_LEDS_16IN, CRGB::White);
  fill_solid( leds3, NUM_LEDS_10IN, CRGB::White);
}

void strobingWhite() {

  fill_gradient_RGB( leds0, NUM_LEDS_10IN, CRGB::Blue, CRGB::Red);
  fill_gradient_RGB( leds1, NUM_LEDS_16IN, CRGB::Yellow, CRGB::Blue);
  fill_gradient_RGB( leds2, NUM_LEDS_16IN, CRGB::Purple, CRGB::Black);
  fill_gradient_RGB( leds3, NUM_LEDS_10IN, CRGB::Orange, CRGB::Green);
}


void comfortWhite() {




    fill_palette(leds0, NUM_LEDS_10IN, wallCounterFast, 255 / NUM_LEDS_10IN, WarmWhites_p, led0Bri, LINEARBLEND);
    fill_palette(leds1, NUM_LEDS_16IN, wallCounterFast * 2, 255 / NUM_LEDS_16IN, WarmWhites_p, led1Bri, LINEARBLEND);
    fill_palette(leds2, NUM_LEDS_16IN, wallCounterFast, 255 / NUM_LEDS_16IN, WarmWhites_p, led2Bri, LINEARBLEND);
    fill_palette(leds3, NUM_LEDS_10IN, wallCounterFast * 3, 255 / NUM_LEDS_10IN, WarmWhites_p, led3Bri, LINEARBLEND);
}

uint8_t adjustBrightness(uint8_t preValue) {

    // to reduce scorekeeping, we'll just use
    // odd number - going up
    // even number - going down

    // if odd number
    if(preValue % 2 == 1) {
        // are we maxed out?
        if(preValue == 255) {
            // set to 252 and go on
            return 252;
        } else {
            // add random value between 2-6
            return qadd8(preValue, getRandomEvenNumber(1,4));
        }
    // if even number
    } else {
        // are we at minimum?
        if(preValue <= 76) {
            // set to 129 and move on
            return 71;
        } else {
            return qsub8(preValue, getRandomEvenNumber(1,4));
        }
    }
}

uint8_t getRandomEvenNumber(uint8_t min, uint8_t max) {
    uint8_t preVal = random8(min, max);
    return (preVal + 1) & ~1;

}

void pearl()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds0, NUM_LEDS_10IN, wallCounterFast, PEARL_RAINBOW_DELTA);
//  leds0.nscale8_video(led0Bri);

  fill_rainbow( leds1, NUM_LEDS_16IN, wallCounterSlow, PEARL_RAINBOW_DELTA);
//  leds0.nscale8_video(led1Bri);
  fill_rainbow( leds2, NUM_LEDS_16IN, wallCounterFast, PEARL_RAINBOW_DELTA);
  fill_rainbow( leds3, NUM_LEDS_10IN, wallCounterSlow, PEARL_RAINBOW_DELTA);
}
