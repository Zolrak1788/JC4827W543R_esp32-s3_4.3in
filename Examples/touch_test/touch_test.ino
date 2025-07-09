#include <XPT2046_Touchscreen.h> // Include the library for the XPT2046 touchscreen controller
#include <SPI.h>                // Include the SPI library for communication with the touchscreen

#define CS_PIN 38               // Define the Chip Select (CS) pin for the touchscreen
#define TIRQ_PIN 3              // Define the Touch Interrupt (TIRQ) pin (optional, for touch detection)

XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN); // Initialize touchscreen object with CS and TIRQ pins

void setup() {
  Serial.begin(115200);         // Start serial communication at 115200 baud rate for debugging
  SPI.begin(12, 13, 11, CS_PIN); // Initialize SPI with pins: SCK=12, MISO=13, MOSI=11, CS=CS_PIN
  ts.begin();                   // Initialize the touchscreen
  pinMode(CS_PIN, OUTPUT);      // Set the CS pin as an output
  digitalWrite(CS_PIN, HIGH);   // Set CS pin HIGH to deselect the touchscreen initially
}

void loop() {
  if (ts.touched()) {           // Check if the touchscreen is touched
    TS_Point p = ts.getPoint(); // Get the touch coordinates and pressure (x, y, z)
    Serial.print("X=");         // Print "X=" to the Serial Monitor
    Serial.print(p.x);          // Print the x-coordinate of the touch
    Serial.print(" Y=");        // Print " Y=" to the Serial Monitor
    Serial.print(p.y);          // Print the y-coordinate of the touch
    Serial.print(" Z=");        // Print " Z=" to the Serial Monitor
    Serial.println(p.z);        // Print the pressure (z) of the touch and move to a new line
  }
  delay(100);                   // Wait 100ms before checking for touch again
}
