// Include library references and their source for the XPT2046 touchscreen controller
/*******************************************************************************
 * Touch libraries:
 * XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
 ******************************************************************************/

// Define that the XPT2046 touchscreen is being used
#define TOUCH_XPT2046

// Define SPI pins for the XPT2046 touchscreen
#define TOUCH_XPT2046_SCK 12   // Clock pin for SPI communication
#define TOUCH_XPT2046_MISO 13  // MISO (Master In Slave Out) pin for SPI
#define TOUCH_XPT2046_MOSI 11  // MOSI (Master Out Slave In) pin for SPI
#define TOUCH_XPT2046_CS 38    // Chip select pin to activate the touchscreen
#define TOUCH_XPT2046_INT 3    // Interrupt pin for touch detection (TIRQ)
#define TOUCH_XPT2046_ROTATION 0  // Set touchscreen rotation to 0 (default/portrait)
#define TOUCH_XPT2046_SAMPLES 50  // Number of samples to take for touch precision

// Include libraries for the XPT2046 touchscreen and SPI communication
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// Initialize an SPIClass object for the touchscreen using the HSPI peripheral
static SPIClass touchSPI(HSPI);

// Create an XPT2046 touchscreen object, specifying chip select and interrupt pins
XPT2046_Touchscreen ts(TOUCH_XPT2046_CS, TOUCH_XPT2046_INT);

// Global variables for touchscreen handling
bool touch_swap_xy = false;  // Flag to swap X and Y coordinates (false = no swap)
int16_t touch_map_x1 = 263;  // Raw X max value maps to screen X min (0)
int16_t touch_map_x2 = 3882; // Raw X min value maps to screen X max (479)
int16_t touch_map_y1 = 365;  // Raw Y min value maps to screen Y min (0)
int16_t touch_map_y2 = 3757; // Raw Y max value maps to screen Y max (271)

int16_t touch_max_x = 0, touch_max_y = 0; // Maximum screen coordinates (set later)
int16_t touch_raw_x = 0, touch_raw_y = 0; // Raw touch coordinates from the controller
int16_t touch_last_x = 0, touch_last_y = 0; // Calibrated (mapped) touch coordinates

// Function to initialize and calibrate the touchscreen based on screen size and rotation
void touch_init(int16_t w, int16_t h, uint8_t r)
{
    touch_max_x = w - 1;  // Set max X coordinate (e.g., 479 for 480px width)
    touch_max_y = h - 1;  // Set max Y coordinate (e.g., 271 for 272px height)

    // Initialize SPI for the touchscreen with specified pins
    touchSPI.begin(TOUCH_XPT2046_SCK, TOUCH_XPT2046_MISO, TOUCH_XPT2046_MOSI, TOUCH_XPT2046_CS);
    
    // Initialize the XPT2046 touchscreen library with the SPI interface
    ts.begin(touchSPI);
    
    // Set the touchscreen rotation (0 = default/portrait)
    ts.setRotation(TOUCH_XPT2046_ROTATION);
}

// Check if the touchscreen interrupt pin (TIRQ) indicates a touch event
bool touch_has_signal()
{
    return ts.tirqTouched(); // Returns true if the TIRQ pin is low, indicating a touch
}

// Translate raw touch coordinates to calibrated screen coordinates
void translate_touch_raw()
{
    if (touch_swap_xy) // If X and Y coordinates need to be swapped
    {
        // Map raw Y to screen X and raw X to screen Y
        touch_last_x = map(touch_raw_y, touch_map_x1, touch_map_x2, 0, touch_max_x);
        touch_last_y = map(touch_raw_x, touch_map_y1, touch_map_y2, 0, touch_max_y);
    }
    else // No swap, map raw coordinates directly
    {
        // Map raw X to screen X and raw Y to screen Y
        touch_last_x = map(touch_raw_x, touch_map_x1, touch_map_x2, 0, touch_max_x);
        touch_last_y = map(touch_raw_y, touch_map_y1, touch_map_y2, 0, touch_max_y);
    }
}

// Detect touch, read multiple samples for accuracy, and update coordinates
bool touch_touched()
{
    if (ts.touched()) // Check if a touch is detected
    {
        // Get the initial touch point (x, y, and pressure z)
        TS_Point p = ts.getPoint();
        touch_raw_x = p.x; // Store raw X coordinate
        touch_raw_y = p.y; // Store raw Y coordinate
        int max_z = p.z;  // Store initial pressure value
        int count = 0;    // Counter for samples

        // Read up to TOUCH_XPT2046_SAMPLES (50) touch points while touched
        while ((ts.touched()) && (count < TOUCH_XPT2046_SAMPLES))
        {
            count++;
            TS_Point p = ts.getPoint(); // Get new touch point
            if (p.z > max_z) // If new pressure is higher, update coordinates
            {
                touch_raw_x = p.x; // Update raw X with highest pressure point
                touch_raw_y = p.y; // Update raw Y with highest pressure point
                max_z = p.z;       // Update max pressure
            }
        }
        translate_touch_raw(); // Convert raw coordinates to screen coordinates
        return true; // Return true to indicate a valid touch
    }
    return false; // Return false if no touch is detected
}

// Always return true for XPT2046 (touch release detection not explicitly handled)
bool touch_released()
{
    return true; // XPT2046 does not use distinct release detection
}
