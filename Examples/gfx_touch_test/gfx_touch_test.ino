// Include the Arduino GFX Library for display handling
#include <Arduino_GFX_Library.h>
// Include the display configuration header
#include "display.h"
// Include the touch controller configuration header
#include "touch.h"

// Setup function runs once at startup
void setup() {
  // Set the backlight pin as an output
  pinMode(GFX_BL, OUTPUT);
  // Turn on the backlight by setting the pin HIGH
  digitalWrite(GFX_BL, HIGH);

  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  // Wait 500ms to ensure serial is ready
  delay(500);

  // Initialize the display first
  if (!gfx->begin()) {
    // Print error message if display initialization fails
    Serial.println("ERROR initializing display");
    // Halt execution in an infinite loop
    while (1);
  }
  // Print success message if display initializes correctly
  Serial.println("Display initialized OK");

  // Initialize the touch controller after the display
  // Initialize touch with display dimensions (480x272) and rotation (0)
  touch_init(480, 272, 0);
  // Print success message for touch initialization
  Serial.println("Touch initialized");

  // Configure initial display content
  // Fill the screen with black color
  gfx->fillScreen(BLACK);
  // Set text color to white
  gfx->setTextColor(WHITE);
  // Set cursor position for text at (50, 50)
  gfx->setCursor(50, 50);
  // Set text size to 2x scale
  gfx->setTextSize(2);
  // Print instruction message on the screen
  gfx->println("Touch the screen to test");
  // Flush the canvas to display the content
  gfx->flush();
}

// Loop function runs repeatedly after setup
void loop() {
  // Check if the screen is touched
  if (touch_touched()) {
    // Get the touch point data (x, y, pressure)
    TS_Point p = ts.getPoint();
    // Filter touch events based on pressure (between 200 and 2000)
    if (p.z >= 200 && p.z <= 2000) {
      // Print raw touch coordinates and pressure to serial
      Serial.print("Touch detected - Raw: (x=");
      Serial.print(touch_raw_x);
      Serial.print(", y=");
      Serial.print(touch_raw_y);
      Serial.print(", z=");
      Serial.print(p.z);
      // Print mapped (calibrated) touch coordinates
      Serial.print(") Mapped: (");
      Serial.print(touch_last_x);
      Serial.print(", ");
      Serial.print(touch_last_y);
      Serial.println(")");
      // Draw a circle at the touch point if coordinates are within display bounds
      if (touch_last_x >= 0 && touch_last_x < 480 && touch_last_y >= 0 && touch_last_y < 272) {
        // Draw a white filled circle with radius 5 at the touch point
        gfx->fillCircle(touch_last_x, touch_last_y, 5, WHITE);
        // Flush the canvas to update the display
        gfx->flush();
      } else {
        // Print warning if mapped coordinates are out of range
        Serial.println("Mapped coordinates out of range, not drawing.");
      }
    } else {
      // Print message if touch pressure is invalid
      Serial.print("Touch ignored: invalid pressure (z=");
      Serial.print(p.z);
      Serial.println(")");
    }
  }
  // Delay 50ms to avoid overwhelming the loop
  delay(50);
}
