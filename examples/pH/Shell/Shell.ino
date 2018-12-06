/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This allows you to run various functions on a command-line like interface.
   For more information and documentation, visit ufire.co

   This is compatible with hardware version 2.

   Display config information
    config (or `c`)

   Basic usage:
    mv
    ph

   Enable temperature compensation
    tc 1

   Tempearture compensation using an outside source of temperature
    tc 1
    ph 20.2

   Single Point Calibration
    cal 7.0

   Dual Point Calibration (in mV)
    low 4.01
    high 7.0
    dp 1

   Take a temperature reading
    temp (or `t`)

   Set a temperature to use:
   t 20 < to use 20 C as the temperature rather than using the attached temp.sensor>

   Change the I2C address
    i2c 3d <address is interpreted as a hex number without the 0x>

   Reset device configuration
    reset (or `r`)
 */

#include <ISE_pH.h>

ISE_pH pH;
String buffer, cmd, p1, p2;

void config() {
  Serial.println("ISE Config:");
  Serial.print("  offset: "); Serial.println(pH.getCalibrateOffset(), 4);
  Serial.print("  dual point: "); Serial.println(pH.usingDualPoint(), 4);
  Serial.print("    low reference | read: "); Serial.print(pH.getCalibrateLowReference(), 4);
  Serial.print(" | "); Serial.println(pH.getCalibrateLowReading(), 4);
  Serial.print("    high reference | read: "); Serial.print(pH.getCalibrateHighReference(), 4);
  Serial.print(" | "); Serial.println(pH.getCalibrateHighReading(), 4);
  Serial.print("  temp. compensation: "); Serial.println(pH.usingTemperatureCompensation());
  Serial.print("  version: 0x"); Serial.println(pH.getVersion(), HEX);
}

void reset_config() {
  pH.reset();
  config();
}

void temperature() {
  pH.measureTemp();
  Serial.print("C|F: "); Serial.print(pH.tempC);
  Serial.print(" | "); Serial.println(pH.tempF);
}

void calibrate() {
  if (p1.length()) {
    pH.calibrateSingle(p1.toFloat());
  }

  Serial.print("offset: ");
  Serial.println(pH.getCalibrateOffset(), 5);
}

void mv() {
  // for (;;) {
  pH.measuremV();

  Serial.print("mV: ");
  Serial.println(pH.mV, 4);

  // }
}

void ph() {
  // for (;;) {
  if (p1.length()) {
    pH.measurepH(p1.toFloat());
  }
  else {
    pH.measurepH();
  }

  Serial.print("pH: ");
  Serial.println(pH.pH, 4);

  // }
}

void data() {
  Serial.print("mV: ");
  Serial.println(pH.mV);
  Serial.print("C|F: "); Serial.print(pH.tempC);
  Serial.print(" |  "); Serial.println(pH.tempF);
}

void low() {
  if (p1.length()) {
    pH.calibrateProbeLow(p1.toFloat());
  }

  Serial.print("low reference | read: "); Serial.print(pH.getCalibrateLowReference(), 2);
  Serial.print(" | "); Serial.println(pH.getCalibrateLowReading(), 2);
}

void high() {
  if (p1.length()) {
    pH.calibrateProbeHigh(p1.toFloat());
  }

  Serial.print("high reference | read: "); Serial.print(pH.getCalibrateHighReference(), 2);
  Serial.print(" | "); Serial.println(pH.getCalibrateHighReading(), 2);
}

void temp_comp() {
  if (p1.length()) {
    pH.useTemperatureCompensation(p1.toInt());
  }

  Serial.print("temp. compensation: ");
  Serial.println(pH.usingTemperatureCompensation());
}

void dual_point() {
  if (p1.length()) {
    pH.useDualPoint(p1.toInt());
  }

  Serial.print("dual point: ");
  Serial.println(pH.usingDualPoint());
}

void i2c() {
  if (p1.length()) {
    pH.setI2CAddress(p1.toInt());
  }
}

void read() {
  if (p1.length()) {
    Serial.println(pH.readEEPROM(p1.toInt()));
  }
}

void write() {
  if (p1.length()) {
    pH.writeEEPROM(p1.toInt(), p2.toInt());
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
