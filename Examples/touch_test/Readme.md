This Arduino sketch demonstrates how to interface with an XPT2046 touchscreen controller using the `XPT2046_Touchscreen` library. The sketch detects touch events on a touchscreen and prints the touch coordinates (`x`, `y`) and pressure (`z`) to the Serial Monitor.

The code initializes the touchscreen via SPI communication and continuously checks for touch input. When a touch is detected, it retrieves the coordinates and pressure, then outputs them to the Serial Monitor for debugging or further processing.
