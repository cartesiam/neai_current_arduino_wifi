# neai_current_arduino_wifi
This tutorial is an optional addition to this one: https://github.com/cartesiam/neai_arduino_current and should only be done afterwards.

## Goal
The goal of this tutorial is to receive notifications from our Arduino Nano IoT 33 when an anomaly is detected.
You should already have accomplished all the steps of the first tutorial and have NEAI library in Arduino libraries.

## Hardware requirements
The base requirements do not change from part 1. However we recommend setting a battery on your arduino for ease of use. 

## Software requirements
Install wifinina library (using the IDE library manager)
Install ArduinoJson library (using the IDE library manager)
Install Sleepydog library from adafruit (using the IDE library manager)

Install push-safer library
https://github.com/appzer/pushsafer-arduino-library
Download the Zip
Extract into your Arduino libraries directory (/documents/Arduino/Libraries in Windows)
Rename the directory from "pushsafer-arduino-library-master" to "Pushsafer"

Create a free account on pushsafer.com
Install the iphone App pushsafer on your iphone (or android)

In the "arduino_secrets.h" set your Wifi (WPA)
SECRET_SSID and SECRET_PASS
From your web account copy the private key and past it in the "arduino_secrets.h" file

### Receive phone notifications from your vacuum
Flash the code to your Nano IoT 33 board. This should let you receive a notification when an anomaly is detected by the board. 
