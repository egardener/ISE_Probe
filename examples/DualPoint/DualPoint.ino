/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

#include "uFire_ISE.h"

ISE_Probe mv;

void calibrateLow() {
  Serial.println("Put the probe in the reference low solution and wait for the readings to stabilize.");
  Serial.println("Enter what the low solution's measurement should be in mV and press enter.");

  while (Serial.available() == 0) {
    mv.measuremV();
    Serial.print("low mV: "); Serial.println(mv.mV);
  }

  mv.calibrateProbeLow(Serial.readStringUntil('/r').toFloat());
  Serial.print("low reference / read: "); Serial.print(mv.getCalibrateLowReference(), 2);
  Serial.print(" / "); Serial.println(mv.getCalibrateLowReading(), 2);
}

void calibrateHigh() {
  Serial.println("Put the probe in the reference high solution and wait for the readings to stabilize.");
  Serial.println("Enter what the high solution's measurement should be in mV and press enter.");

  while (Serial.available() == 0) {
    mv.measuremV();
    Serial.print("high mV: "); Serial.println(mv.mV);
  }

  mv.calibrateProbeHigh(Serial.readStringUntil('/r').toFloat());
  Serial.print("high reference / read: "); Serial.print(mv.getCalibrateHighReference(), 2);
  Serial.print(" / "); Serial.println(mv.getCalibrateHighReading(), 2);
}

void setup() {
  Serial.begin(9600);
  calibrateLow();
  calibrateHigh();
  mv.useDualPoint(true);
}

void loop() {
  mv.measuremV();
  Serial.print("mV: "); Serial.println(mv.mV);
}
