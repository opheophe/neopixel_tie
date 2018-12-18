#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define light_pin 10
#define pot_pin 1
#define num_pixels 1

int brightness = 100;
long int setup_millis = 0;
int mapped = 0;
int mode=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_pixels, light_pin, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {





  if (setup_millis + 5000 > millis()) {
    brightness = map(analogRead(pot_pin), 0, 1024, 1, 101);
    Serial.print("Setting brightness: ");
    Serial.println(brightness);
    colorWipe(strip.Color(255 * brightness / 100, 255 * brightness / 100, 0), 0);
  } else {

    mapped = map(analogRead(pot_pin), 0, 1024, 0, 7 + 1);
    Serial.print("Mapping: ");
    Serial.println(mapped);

    switch (mapped) {
      case 0:
        Serial.println("Setup");

        setup_millis = millis();
        break;
      case 1:
        colorWipe(strip.Color(255 * brightness / 100, 0, 0), 50); // Red
        colorWipe(strip.Color(0, 0, 0), 10);
        break;
      case 2:
        colorWipe(strip.Color(0, 255 * brightness / 100, 0), 50); // Green
        colorWipe(strip.Color(0, 0, 0), 10);
        break;
      case 3:
        colorWipe(strip.Color(0, 0, 255 * brightness / 100), 50); // Blue
        colorWipe(strip.Color(0, 0, 0), 10);
        break;
      case 4:
        colorWipe(strip.Color(255 * brightness / 100, 0, 0), 50); // Red
        colorWipe(strip.Color(0, 255 * brightness / 100, 0), 50); // Green
        colorWipe(strip.Color(0, 0, 255 * brightness / 100), 50); // Blue
      case 5:
        rainbow(10);
        break;
      case 6:
        rainbowCycle(20);
        break;
      case 7:
        theaterChaseRainbow(20);
        break;
      default:
        break;

    }
  }



  //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  // theaterChase(strip.Color(127, 127, 127), 50); // White
  // theaterChase(strip.Color(127, 0, 0), 50); // Red
  // theaterChase(strip.Color(0, 0, 127), 50); // Blue

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * brightness / 100, 0, (WheelPos * 3) * brightness / 100);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) * brightness / 100, (255 - WheelPos * 3) * brightness / 100);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3) * brightness / 100, (255 - WheelPos * 3) * brightness / 100, 0);
}
