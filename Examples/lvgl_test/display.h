// Include the Arduino_GFX_Library, which provides functions to interface with various graphical displays
#include <Arduino_GFX_Library.h>

// Define the specific display device as ESP32_4827A043_QSPI, which tells the library the hardware configuration
#define GFX_DEV_DEVICE ESP32_4827A043_QSPI

// Define the backlight pin (GPIO 1) used to control the display's backlight
#define GFX_BL 1

// Create a new QSPI bus object, specifying the pins for chip select (cs), clock (sck), and data lines (d0-d3)
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    45 /* cs: chip select pin */, 
    47 /* sck: clock pin for QSPI communication */, 
    21 /* d0: data line 0 */, 
    48 /* d1: data line 1 */, 
    40 /* d2: data line 2 */, 
    39 /* d3: data line 3 */
);

// Create a display object for the NV3041A panel, using the QSPI bus, no reset pin (GFX_NOT_DEFINED), 
// 0 rotation (portrait mode), and enabling IPS mode for better color and viewing angles
Arduino_GFX *g = new Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

// Create a virtual canvas (framebuffer in RAM) with 480x272 resolution, linked to the physical display (g)
Arduino_GFX *gfx = new Arduino_Canvas(480 /* width */, 272 /* height */, g);

// Define CANVAS to indicate that the canvas (framebuffer) mode is being used for rendering
#define CANVAS
