/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This allows you to run various functions on a command-line like interface.
   For more information and documentation, visit ufire.co

   Display config information
    config

   Basic usage:
    mv
    ph

   Enable temperature compensation
    tc 1
    temp

   Single Point Calibration
    cal 200

   Dual Point Calibration (in mV)
    low 50
    high 200
    dp 1

   Change the I2C address
    i2c 30

   Reset device configuration
    reset
 */

#include <uFire_ISE.h>
#include <ISE_pH.h>

ISE_Probe mV;
ISE_pH    pH;
String    buffer, cmd, p1, p2;

void config() {
  Serial.println("ISE Config:");
  Serial.print("  offset: "); Serial.println(mV.getCalibrateOffset(), 4);
  Serial.print("  dual point: "); Serial.println(mV.usingDualPoint(), 4);
  Serial.print("    low reference | read: "); Serial.print(mV.getCalibrateLowReference(), 4);
  Serial.print(" | "); Serial.println(mV.getCalibrateLowReading(), 4);
  Serial.print("    high reference | read: "); Serial.print(mV.getCalibrateHighReference(), 4);
  Serial.print(" | "); Serial.println(mV.getCalibrateHighReading(), 4);
  Serial.print("  temp. compensation: "); Serial.println(mV.usingTemperatureCompensation());
  Serial.print("  version: 0x"); Serial.println(mV.getVersion(), HEX);
}

void reset_config() {
  mV.reset();
  config();
}

void temperature() {
  mV.measureTemp();
  Serial.print("C|F: "); Serial.print(mV.tempC);
  Serial.print(" | "); Serial.println(mV.tempF);
}

void calibrate() {
  if (p1.length()) {
    mV.calibrateSingle(p1.toFloat());
  }

  Serial.print("offset: ");
  Serial.println(mV.getCalibrateOffset(), 5);
}

void mv() {
  for (;;) {
    mV.measuremV();

    Serial.print("mV: ");
    Serial.println(mV.mV, 4);
  }
}

void ph() {
  for (;;) {
    pH.measurepH();

    Serial.print("pH: ");
    Serial.println(pH.pH, 4);
  }
}

void data() {
  Serial.print("mV: ");
  Serial.println(mV.mV);
  Serial.print("C|F: "); Serial.print(mV.tempC);
  Serial.print(" |  "); Serial.println(mV.tempF);
}

void low() {
  if (p1.length()) {
    mV.calibrateProbeLow(p1.toFloat());
  }

  Serial.print("low reference | read: "); Serial.print(mV.getCalibrateLowReference(), 2);
  Serial.print(" | "); Serial.println(mV.getCalibrateLowReading(), 2);
}

void high() {
  if (p1.length()) {
    mV.calibrateProbeHigh(p1.toFloat());
  }

  Serial.print("high reference | read: "); Serial.print(mV.getCalibrateHighReference(), 2);
  Serial.print(" | "); Serial.println(mV.getCalibrateHighReading(), 2);
}

void temp_comp() {
  if (p1.length()) {
    mV.useTemperatureCompensation(p1.toInt());
  }

  Serial.print("temp. compensation: ");
  Serial.println(mV.usingTemperatureCompensation());
}

void dual_point() {
  if (p1.length()) {
    mV.useDualPoint(p1.toInt());
  }

  Serial.print("dual point: ");
  Serial.println(mV.usingDualPoint());
}

void i2c() {
  if (p1.length()) {
    mV.setI2CAddress(p1.toInt());
  }
}

void read() {
  if (p1.length()) {
    Serial.println(mV.readEEPROM(p1.toInt()));
  }
}

void write() {
  if (p1.length()) {
    mV.writeEEPROM(p1.toInt(), p2.toInt());
  }
}

void cmd_run() {
  if ((cmd == "conf") || (cmd == "config") || (cmd == "c")) config();
  if ((cmd == "reset") || (cmd == "r")) reset_config();
  if ((cmd == "temp") || (cmd == "t")) temperature();
  if ((cmd == "calibrate") || (cmd == "cal")) calibrate();
  if (cmd == "mv") mv();
  if (cmd == "ph") ph();
  if ((cmd == "data") || (cmd == "d")) data();
  if (cmd == "low") low();
  if (cmd == "high") high();
  if (cmd == "tc") temp_comp();
  if (cmd == "dp") dual_point();
  if (cmd == "i2c") i2c();
  if (cmd == "read") read();
  if (cmd == "write") write();
}

void cli_process() {
  while (Serial.available()) {
    char c = Serial.read();

    switch (c) {
    case '\n': // new line
      Serial.println();
      cmd = buffer.substring(0, buffer.indexOf(" ", 0)); cmd.trim();
      buffer.remove(0, buffer.indexOf(" ", 0)); buffer.trim();
      p1 = buffer.substring(0, buffer.indexOf(" ", 0)); p1.trim();
      buffer.remove(0, buffer.indexOf(" ", 0)); buffer.trim();
      p2 = buffer.substring(0, buffer.indexOf(" ", 0)); p2.trim();
      cmd_run();
      Serial.print("> ");
      buffer = "";
      break;

    case '\b': // backspace
      buffer.remove(buffer.length() - 1);
      Serial.print("\b \b");
      break;

    default: // everything else
      buffer += c;
      Serial.print(c);
    }
  }
}

void setup() {
  Serial.begin(9600);
  config();
  Serial.print("> ");
}

void loop() {
  cli_process();
}
