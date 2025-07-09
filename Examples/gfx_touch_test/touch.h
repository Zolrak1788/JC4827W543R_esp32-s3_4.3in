/*******************************************************************************
 * Touch libraries:
 * XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
 ******************************************************************************/

// Define the use of XPT2046 touch controller
#define TOUCH_XPT2046
// Define SPI pins for the touch controller
#define TOUCH_XPT2046_SCK 12   // SPI clock pin
#define TOUCH_XPT2046_MISO 13  // SPI MISO pin
#define TOUCH_XPT2046_MOSI 11  // SPI MOSI pin
#define TOUCH_XPT2046_CS 38    // Chip select pin
#define TOUCH_XPT2046_INT 3    // Interrupt pin for touch detection
// Define touch rotation (0 = default)
#define TOUCH_XPT2046_ROTATION 0
// Define number of samples to average for touch readings
#define TOUCH_XPT2046_SAMPLES 50

// Include the XPT2046 Touchscreen library
#include <XPT2046_Touchscreen.h>
// Include the SPI library for communication
#include <SPI.h>

// Create an SPI object for the touch controller using HSPI
static SPIClass touchSPI(HSPI);
// Create a touch controller object with chip select and interrupt pins
XPT2046_Touchscreen ts(TOUCH_XPT2046_CS, TOUCH_XPT2046_INT);

// Global variables for touch handling
bool touch_swap_xy = false;          // Flag to swap x and y coordinates based on rotation
int16_t touch_map_x1 = -1;           // Calibration: x-axis raw minimum
int16_t touch_map_x2 = -1;           // Calibration: x-axis raw maximum
int16_t touch_map_y1 = -1;           // Calibration: y-axis raw minimum
int16_t touch_map_y2 = -1;           // Calibration: y-axis raw maximum
int16_t touch_max_x = 0;             // Maximum x-coordinate (display width - 1)
int16_t touch_max_y = 0;             // Maximum y-coordinate (display height - 1)
int16_t touch_raw_x = 0;             // Raw x-coordinate from touch controller
int16_t touch_raw_y = 0;             // Raw y-coordinate from touch controller
int16_t touch_last_x = 0;            // Calibrated (mapped) x-coordinate
int16_t touch_last_y = 0;            // Calibrated (mapped) y-coordinate

// Initialize and calibrate the touch controller based on display size and rotation
void touch_init(int16_t w, int16_t h, uint8_t r) {
  // Set maximum x and y based on display dimensions
  touch_max_x = w - 1;
  touch_max_y = h - 1;

  // Set default calibration values if not already defined
  if (touch_map_x1 == -1) {
    switch (r) {
      case 3: // Rotation 3 (270 degrees)
        touch_swap_xy = true;              // Swap x and y coordinates
        touch_map_x1 = touch_max_x;        // Map x raw max to display x max
        touch_map_x2 = 0;                  // Map x raw min to display x min
        touch_map_y1 = 0;                  // Map y raw min to display y min
        touch_map_y2 = touch_max_y;        // Map y raw max to display y max
        break;
      case 2: // Rotation 2 (180 degrees)
        touch_swap_xy = false;             // No swap needed
        touch_map_x1 = touch_max_x;        // Map x raw max to display x max
        touch_map_x2 = 0;                  // Map x raw min to display x min
        touch_map_y1 = touch_max_y;        // Map y raw max to display y max
        touch_map_y2 = 0;                  // Map y raw min to display y min
        break;
      case 1: // Rotation 1 (90 degrees)
        touch_swap_xy = true;              // Swap x and y coordinates
        touch_map_x1 = 0;                  // Map x raw min to display x min
        touch_map_x2 = touch_max_x;        // Map x raw max to display x max
        touch_map_y1 = touch_max_y;        // Map y raw max to display y max
        touch_map_y2 = 0;                  // Map y raw min to display y min
        break;
      default: // Rotation 0 (0 degrees)
        touch_swap_xy = false;             // No swap needed
        touch_map_x1 = 0;                  // Map x raw min to display x min
        touch_map_x2 = touch_max_x;        // Map x raw max to display x max
        touch_map_y1 = 0;                  // Map y raw min to display y min
        touch_map_y2 = touch_max_y;        // Map y raw max to display y max
        break;
    }
  }

  // Initialize SPI for the touch controller
  touchSPI.begin(TOUCH_XPT2046_SCK, TOUCH_XPT2046_MISO, TOUCH_XPT2046_MOSI, TOUCH_XPT2046_CS);
  // Initialize the touch controller library
  ts.begin(touchSPI);
  // Set the touch controller rotation
  ts.setRotation(TOUCH_XPT2046_ROTATION);
}

// Check if the touch interrupt pin indicates a touch event
bool touch_has_signal() {
  return ts.tirqTouched(); // Return true if touch interrupt is active
}

// Map raw touch coordinates to calibrated display coordinates
void translate_touch_raw() {
  if (touch_swap_xy) {
    // Swap and map x and y if required by rotation
    touch_last_x = map(touch_raw_y, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_x, touch_map_y1, touch_map_y2, 0, touch_max_y);
  } else {
    // Map x and y without swapping
    touch_last_x = map(touch_raw_x, touch_map_x1, touch_map_x2, 0, touch_max_x);
    touch_last_y = map(touch_raw_y, touch_map_y1, touch_map_y2, 0, touch_max_y);
  }
}

// Detect touch events, average multiple samples, and update coordinates
bool touch_touched() {
  if (ts.touched()) { // Check if a touch is detected
    // Get initial touch point
    TS_Point p = ts.getPoint();
    touch_raw_x = p.x; // Store raw x-coordinate
    touch_raw_y = p.y; // Store raw y-coordinate
    int max_z = p.z;   // Store initial pressure
    int count = 0;     // Initialize sample counter

    // Collect additional samples for accuracy
    while ((ts.touched()) && (count < TOUCH_XPT2046_SAMPLES)) {
      count++; // Increment sample counter
      TS_Point p = ts.getPoint(); // Get new touch point
      if (p.z > max_z) { // Update coordinates if pressure is higher
        touch_raw_x = p.x;
        touch_raw_y = p.y;
        max_z = p.z;
      }
    }
    // Map raw coordinates to display coordinates
    translate_touch_raw();
    return true; // Return true to indicate a valid touch
  }
  return false; // Return false if no touch is detected
}

// Always return true for XPT2046 (release detection not implemented)
bool touch_released() {
  return true;
}
