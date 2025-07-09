#include <Arduino_GFX_Library.h>
// Includes libraries for display control and device-specific configurations.

#define GFX_BL 1
// Defines the GPIO pin (1) for the display's backlight.

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    45 /* cs */, 47 /* sck */, 21 /* d0 */, 48 /* d1 */, 40 /* d2 */, 39 /* d3 */);
// Configures QSPI bus with pins for chip select (45), clock (47), and data lines (21, 48, 40, 39).

Arduino_GFX *g = new Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);
// Creates object for NV3041A display, linked to QSPI bus, with no reset pin, default rotation, and IPS mode.

Arduino_GFX *gfx = new Arduino_Canvas(480 /* width */, 272 /* height */, g);
// Creates a 480x272 pixel virtual canvas (framebuffer in RAM) linked to the physical display.

void setup() {
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);  // Sets backlight pin as output and turns it on.

  gfx->begin();  // Initializes the canvas and physical display.

  gfx->fillScreen(0xFFE0);  // Fills canvas with yellow (RGB565: 0xFFE0).

  int rectWidth = 100;
  int rectHeight = 50;
  int rectX = (480 - rectWidth) / 2;  // Calculates x-coordinate to center rectangle horizontally.
  int rectY = (272 - rectHeight) / 2;  // Calculates y-coordinate to center rectangle vertically.
  gfx->fillRect(rectX, rectY, rectWidth, rectHeight, 0x07E0);  // Draws a 100x50 green (RGB565: 0x07E0) rectangle at calculated position.

  gfx->flush();  // Transfers canvas content to the physical display.
}

void loop() {
  // Empty loop; drawing occurs once in setup.
}
