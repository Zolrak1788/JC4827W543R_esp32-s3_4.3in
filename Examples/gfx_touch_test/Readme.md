This Arduino sketch demonstrates the use of a 480x272 touchscreen display with an ESP32 microcontroller. The display uses the NV3041A driver with a QSPI interface, and the touchscreen is controlled by the XPT2046 touch controller. The sketch initializes the display, sets up the touch controller, and allows users to draw white circles on the screen by touching it. Touch coordinates (raw and mapped) and pressure values are printed to the Serial Monitor for debugging.

The device is uncalibrated as in the touch.h file the variables touch_map are with -1 values.

```
static SPIClass touchSPI(HSPI);
```
This line creates a dedicated SPI interface for the XPT2046 touchscreen controller, separate from other SPI devices (like the display, which uses QSPI in this sketch). By using the HSPI module, it ensures that the touchscreen communicates over specific pins (defined later as TOUCH_XPT2046_SCK, TOUCH_XPT2046_MISO, TOUCH_XPT2046_MOSI, and TOUCH_XPT2046_CS).

