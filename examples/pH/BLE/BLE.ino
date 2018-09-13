/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   1. This example is compatible with ISE Probe Interface hardware board version 2.
   2. This is for an ESP32 board.
   3. The 'ESP32 BLE Arduino' library needs to be installed.
   4. Connect the ISE Probe Interface to your ESP32 3.3/5v to VCC, GND to GND, SDA to 19 and SCL to 23
   5. After uploading this sketch, you can see the results by going to https://ufire.co/uFire_BLE/ in the Chrome browser
 */

#include "uFire_pH_BLE.h"

// On the ESP32, the I2C pins can be chosen. In this case, sda=19 and scl=23
uFire_pH_BLE ph_ble(19, 23);

void setup() {
  // start the BLE server
  ph_ble.startBLE();
}

void loop() {
  // loop through and take continous measurements
  ph_ble.measurepH();
  ph_ble.measureTemp();
}
