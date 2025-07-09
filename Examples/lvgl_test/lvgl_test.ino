// Include the LVGL library for creating the graphical user interface
#include <lvgl.h>

// Include device.h for display configuration (Arduino_GFX with NV3041A and QSPI bus)
#include "display.h"

// Include touch.h for touch controller configuration (XPT2046 with HSPI)
#include "touch.h"

// Define screen dimensions (480x272 pixels, matching the NV3041A display)
const int16_t screenWidth = 480;
const int16_t screenHeight = 272;

// Declare LVGL display buffer to hold pixel data for rendering (1/10th of screen size)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

// Declare global variables for a counter and a label to display it
static int counter = 0; // Tracks button presses
static lv_obj_t *counter_label; // LVGL object for the counter display

/* LVGL Flush Callback
 * This function tells LVGL how to send rendered pixel data to the display.
 * It uses the 'gfx' object from display.h to draw the image on the screen.
 */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    // Check if gfx (display driver) or color_p (pixel data) is null to avoid crashes
    if (!gfx || !color_p) {
        Serial.println("Error: gfx or color_p is null!");
        lv_disp_flush_ready(disp_drv); // Inform LVGL that flushing is complete
        return;
    }
    // Calculate the width and height of the area to be drawn
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    // Draw the pixel data to the display using the Arduino_GFX library
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, w, h);
    // Inform LVGL that the frame is ready for the next render
    lv_disp_flush_ready(disp_drv);
}

/* LVGL Touch Read Callback
 * This function reads touch input from the XPT2046 controller (via touch.h).
 * It maps raw touch coordinates to screen coordinates and updates LVGL's input state.
 */
void my_touch_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    // Check if the screen is touched using the touch_touched() function from touch.h
    if (touch_touched()) {
        // Set the touch coordinates (mapped to screen space in touch.h)
        data->point.x = touch_last_x;
        data->point.y = touch_last_y;
        // Indicate that the screen is being pressed
        data->state = LV_INDEV_STATE_PRESSED;
        // Print touch coordinates to the serial monitor for debugging
        Serial.printf("Touch at x=%d, y=%d\n", touch_last_x, touch_last_y);
    } else {
        // Indicate that the screen is not being pressed
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/* Button Event Callback
 * This function is called when the button is clicked (touched).
 * It increments the counter and updates the counter label on the screen.
 */
void btn_event_cb(lv_event_t *e) {
    counter++; // Increment the counter
    char buf[32]; // Buffer for the counter text
    snprintf(buf, sizeof(buf), "Count: %d", counter); // Format the counter value
    lv_label_set_text(counter_label, buf); // Update the counter label
    Serial.println(buf); // Print the counter value to the serial monitor
}

void setup() {
    // Initialize the serial monitor at 115200 baud for debugging
    Serial.begin(115200);

    // Print free heap memory before initialization for memory usage tracking
    Serial.printf("Free heap before: %d bytes\n", ESP.getFreeHeap());

    // Initialize the display driver (gfx is an Arduino_Canvas wrapping Arduino_NV3041A)
    gfx->begin(); // Sets up the QSPI bus and display (calls g->begin() internally)
    // Check if gfx initialized correctly to avoid null pointer issues
    if (!gfx) {
        Serial.println("Error: gfx is null!");
        while (1); // Halt if initialization fails
    }
    // Set up the backlight pin and turn it on
    pinMode(GFX_BL, OUTPUT); // GFX_BL is defined as pin 1 in display.h
    digitalWrite(GFX_BL, HIGH); // Enable backlight

    // Initialize the touch controller (XPT2046 with HSPI) from touch.h
    touch_init(screenWidth, screenHeight, 0); // Sets up HSPI and calibration

    // Initialize the LVGL library
    lv_init();

    // Allocate the LVGL display buffer for rendering (1/10th of screen size, ~9.6 KB)
    buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 10, MALLOC_CAP_DMA);
    // Check if the buffer allocation was successful
    if (!buf) {
        Serial.println("LVGL buffer allocation failed!");
        while (1); // Halt if allocation fails
    }
    // Initialize the LVGL display buffer with the allocated memory
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    // Set up the LVGL display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); // Initialize driver structure
    disp_drv.hor_res = screenWidth; // Set horizontal resolution (480)
    disp_drv.ver_res = screenHeight; // Set vertical resolution (272)
    disp_drv.flush_cb = my_disp_flush; // Link the flush callback
    disp_drv.draw_buf = &draw_buf; // Link the display buffer
    lv_disp_drv_register(&disp_drv); // Register the driver with LVGL

    // Set up the LVGL input device driver for touch
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); // Initialize input driver structure
    indev_drv.type = LV_INDEV_TYPE_POINTER; // Set input type to touch (pointer)
    indev_drv.read_cb = my_touch_read; // Link the touch read callback
    lv_indev_drv_register(&indev_drv); // Register the input driver with LVGL

    // Create a button on the active screen
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0); // Center the button
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL); // Add click callback
    // Create a label inside the button
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Hello!"); // Set button text
    lv_obj_center(label); // Center the label within the button

    // Create a label to display the counter
    counter_label = lv_label_create(lv_scr_act());
    lv_label_set_text(counter_label, "Count: 0"); // Initialize counter text
    lv_obj_align(counter_label, LV_ALIGN_TOP_MID, 0, 20); // Position at top center

    // Print setup completion and final heap memory
    Serial.println("Setup complete. UI should be visible.");
}

void loop() {
    // Run LVGL's task handler to process UI updates, animations, and input
    lv_timer_handler();
    // Flush the canvas framebuffer to the physical display
    gfx->flush();
    // Small delay to prevent overloading the CPU (5ms is typical for LVGL)
    delay(5);
}
