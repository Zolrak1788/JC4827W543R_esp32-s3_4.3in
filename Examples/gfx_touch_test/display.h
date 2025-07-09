// Include the Arduino GFX Library for display handling
#include <Arduino_GFX_Library.h>

// Define the specific display device (ESP32-based with QSPI interface)
#define GFX_DEV_DEVICE ESP32_4827A043_QSPI

// Define the backlight pin for the display
#define GFX_BL 1

// Create a QSPI bus object for communication with the display
// Parameters: cs (chip select), sck (clock), d0-d3 (data lines)
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    45 /* cs */, 47 /* sck */, 21 /* d0 */, 48 /* d1 */, 40 /* d2 */, 39 /* d3 */);

// Create a display object for the NV3041A physical display
// Parameters: bus (QSPI bus), RST (reset pin, not defined), rotation (0), IPS (true for IPS display)
Arduino_GFX *g = new Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

// Create a virtual canvas (framebuffer in RAM) for rendering graphics
// Parameters: width (480 pixels), height (272 pixels), output display (g)
Arduino_GFX *gfx = new Arduino_Canvas(480 /* width */, 272 /* height */, g);

// Define CANVAS to indicate the use of a virtual canvas
#define CANVAS
