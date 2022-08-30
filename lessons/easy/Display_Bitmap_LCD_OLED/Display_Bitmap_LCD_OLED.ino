#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   32
#define LOGO_WIDTH    64
static const unsigned char PROGMEM logo_anakkendali[] =
{ B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00011111, B11111111, B11111111, B11111000, B00000000, B00000000, 
  B00000000, B00000001, B11111111, B11111111, B11111111, B11111111, B10000000, B00000000, 
  B00000000, B00001111, B11111111, B11111111, B11111111, B11111111, B11110000, B00000000, 
  B00000000, B00111111, B11111111, B10000000, B00111111, B11111111, B11111100, B00000000, 
  B00000000, B11111111, B11111000, B00000000, B00011111, B11111111, B11111111, B00000000, 
  B00000011, B11111111, B11000000, B00000000, B00011111, B11111111, B11111111, B11000000, 
  B00000111, B11111111, B10000000, B01111000, B00011111, B11111111, B11111111, B11100000, 
  B00001111, B11111110, B00000011, B11111000, B00011111, B11111111, B11111111, B11110000, 
  B00011111, B11111100, B00001111, B11111000, B00011111, B11111111, B11111111, B11111000, 
  B00111111, B11111000, B00011111, B11111000, B00011111, B11111111, B11111111, B11111100, 
  B01111111, B11110000, B00111111, B11111000, B00011111, B11111111, B11111111, B11111110, 
  B01111111, B11100000, B00111111, B11111000, B00011111, B11111111, B11111111, B11111110, 
  B11111100, B00000000, B00111111, B11111000, B00011111, B11111111, B11111111, B11111111, 
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, 
  B11111111, B11111111, B01110111, B11111111, B11111111, B11111111, B11111111, B11111111, 
  B11111111, B11110111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, 
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, 
  B11111111, B11111111, B11111111, B11111000, B00011111, B11111100, B00000000, B00111111, 
  B01111111, B11111111, B11111111, B11111000, B00011111, B11111100, B00000111, B11111110, 
  B01111111, B11111111, B11111111, B11111000, B00011111, B11111100, B00001111, B11111110, 
  B00111111, B11111111, B11111111, B11111000, B00011111, B11111000, B00011111, B11111100, 
  B00011111, B11111111, B11111111, B11111000, B00011111, B11110000, B00111111, B11111000, 
  B00001111, B11111111, B11111111, B11111000, B00011111, B11000000, B01111111, B11110000, 
  B00000111, B11111111, B11111111, B11111000, B00011111, B00000000, B11111111, B11100000, 
  B00000011, B11111111, B11111111, B11111000, B00000000, B00000011, B11111111, B11000000, 
  B00000000, B11111111, B11111111, B11111000, B00000000, B00001111, B11111111, B00000000, 
  B00000000, B00111111, B11111111, B11111100, B00000000, B11111111, B11111100, B00000000, 
  B00000000, B00001111, B11111111, B11111111, B11111111, B11111111, B11110000, B00000000, 
  B00000000, B00000001, B11111111, B11111111, B11111111, B11111111, B10000000, B00000000, 
  B00000000, B00000000, B00011111, B11111111, B11111111, B11111000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,  
};

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  delay(2000);
  display.clearDisplay();
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("anakkendali.com");
  display.display();
  delay(2000);
  display.clearDisplay();
  testdrawbitmap();
  delay(3000);
  testanimate(logo_anakkendali, LOGO_WIDTH, LOGO_HEIGHT);
}

void loop() {
}
void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_anakkendali, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 
#define YPOS   1
#define DELTAY 2
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}
