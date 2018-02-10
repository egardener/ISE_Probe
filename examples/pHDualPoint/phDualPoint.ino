/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

 #include "ISE_pH.h"

ISE_pH ph;

void calibrateLow() {
  Serial.println("Put the probe in the reference low solution and wait for the readings to stabilize.");
  Serial.println("Enter what the low solution's measurement should be in pH and press enter.");

  while (Serial.available() == 0) {
    ph.measurepH();
    Serial.print("low pH: "); Serial.println(ph.pH);
  }

  ph.calibrateProbeLow(ph.pHtomV(Serial.readStringUntil('/r').toFloat()));
  Serial.print("low reference / read: "); Serial.print(ph.getCalibrateLowReference(), 2);
  Serial.print(" / "); Serial.println(ph.getCalibrateLowReading(), 2);
}

void calibrateHigh() {
  Serial.println("Put the probe in the reference high solution and wait for the readings to stabilize.");
  Serial.println("Enter what the high solution's measurement should be in pH and press enter.");

  while (Serial.available() == 0) {
    ph.measuremV();
    Serial.print("high pH: "); Serial.println(ph.pH);
  }

  ph.calibrateProbeHigh(ph.pHtomV(Serial.readStringUntil('/r').toFloat()));
  Serial.print("high reference / read: "); Serial.print(ph.getCalibrateHighReference(), 2);
  Serial.print(" / "); Serial.println(ph.getCalibrateHighReading(), 2);
}

void setup() {
  Serial.begin(9600);
  calibrateLow();
  calibrateHigh();
  ph.useDualPoint(true);
}

void loop() {
  ph.measurepH();
  Serial.print("pH: "); Serial.println(ph.pH);
}
