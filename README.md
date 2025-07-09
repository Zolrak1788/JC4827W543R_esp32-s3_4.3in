# JC4827W543R: ESP32-S3 with Resistive Touch Display



This repository contains firmware for the JC4827W543 R, an ESP32-S3 board with a 480x272 resistive touchscreen display (NV3041A driver, XPT2046 touch controller). It uses GFX for Arduino, LVGL 8.4, and XPT2046_Touchscreen libraries for rendering graphics and handling touch input, ideal for IoT dashboards and interactive displays.

## Table of Contents
- [Overview](#overview)
- [Hardware](#hardware)
- [Requirements](#requirements)
- [Examples](#examples)


## Overview
The JC4827W543 R project enables graphical interfaces and touch interactions on an ESP32-S3 board with a 480x272 resistive touchscreen. It leverages LVGL 8.4 for UI rendering, GFX for Arduino for display control, and XPT2046_Touchscreen for resistive touch input, supporting Wi-Fi/Bluetooth for IoT applications.

## Hardware
- **Board**: JC4827W543R (ESP32-S3 with 480x272 resistive touchscreen)
- **Display Driver**: NV3041A (480x272 resolution)
- **Touch Controller**: XPT2046 (resistive touch via SPI)

## Requirements
- **Software**:
  - Arduino IDE (>= 2.0) with [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
  - Libraries: [GFX for Arduino](https://github.com/moononournation/Arduino_GFX), [LVGL 8.4](https://github.com/lvgl/lvgl), [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- **Hardware**:
  - [Guition ESP32-S3 8M PSRAM 4M FLASH 4.3-inch IPS 480*272 LCD display module with WIFI and Bluetooth](https://www.aliexpress.com/item/3256806543063048.html?spm=a2g0o.productlist.main.38.c5a93586kB4KBH&algo_pvid=cc1099d1-f5fe-4f77-a11c-61ede57b9a5e&algo_exp_id=cc1099d1-f5fe-4f77-a11c-61ede57b9a5e-37&pdp_ext_f=%7B%22order%22%3A%22805%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%2110.51%2110.51%21%21%2175.00%2175.00%21%40214100f417520245530601352e1dfc%2112000038112486568%21sea%21EC%210%21ABX&curPageLogUid=6W3MpFTD2tzw&utparam-url=scene%3Asearch%7Cquery_from%3A&gatewayAdapt=4itemAdapt)
  - USB-C cable

## Examples
This examples will cover the basics of the board, display and touch. Only the first example will be coded in one single sketch, the rest of examples will have the touch part and the display part in separate files form the main sketch.
- **Board**: JC4827W543R (ESP32-S3 with 480x272 resistive touchscreen)
- **Display Driver**: NV3041A (480x272 resolution)
- **Touch Controller**: XPT2046 (resistive touch via SPI)
