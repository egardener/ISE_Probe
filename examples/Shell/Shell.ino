/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

#include <pHProbe.h>
#include "CLI.h"

/*
   This allows you to run various functions on a command-line like interface.

   Basic usage:
   1. ph

   Single Point calibration:
   1. cal 7.0
   2. ph

   Dual Point Calibration:
   1. low 7.0
   2. high 9.0
   3. dp 1
   4. ph

 */


CLI_COMMAND(reset);  // 'reset' reset all calibration configuration
CLI_COMMAND(temp);   // 'temp' measure temperature
CLI_COMMAND(cal);    // 'zero 7.0' calibrate the offset of the probe from pH 7
CLI_COMMAND(ph);     // 'ph' starts an pH measurement
CLI_COMMAND(mv);     // 'mv' starts a measurement and returns the mV on the probe
CLI_COMMAND(low);    // 'low 1.0' calibrates the low dual-point calibration settings using low solution
CLI_COMMAND(high);   // 'high 10.0'calibrates the high dual-point calibration settings using high solution
CLI_COMMAND(config); // 'config' shows configuration information
CLI_COMMAND(tc);     // 'tc 0/1' temperature compensation; first argument to use it, second the constant to use
CLI_COMMAND(dp);     // 'dp 0/1' to use dual-point calibration or not
CLI_COMMAND(i2c);    // 'ic2 48' changes the i2c address to 48

pH_Probe pH;

void setup()
{
  Serial.begin(9600);
  delay(1000); Serial.println();
  CLI.setDefaultPrompt("> ");
  CLI.onConnect(config);

  CLI.addCommand("reset",  reset);
  CLI.addCommand("temp",   temp);
  CLI.addCommand("cal",    cal);
  CLI.addCommand("ph",     ph);
  CLI.addCommand("mv",     mv);
  CLI.addCommand("low",    low);
  CLI.addCommand("high",   high);
  CLI.addCommand("config", config);
  CLI.addCommand("tc",     tc);
  CLI.addCommand("dp",     dp);
  CLI.addCommand("i2c",    i2c);

  CLI.addClient(Serial);
}

void loop()
{
  CLI.process();
}

CLI_COMMAND(reset) {
  pH.reset();
  return 0;
}

CLI_COMMAND(temp) {
  pH.measureTemp();
  dev->print("C/F: "); dev->print(pH.tempC);
  dev->print(" /  "); dev->println(pH.tempF);
  return 0;
}

CLI_COMMAND(cal) {
  if (argc == 2) {
    pH.calibrateSingle(atof(argv[1]));
  }

  dev->print("offset: ");
  dev->print(pH.getCalibrateOffset());
  return 0;
}

CLI_COMMAND(ph) {
  pH.measurepH();
  dev->print("pH: ");
  dev->print(pH.pH, 2);
  return 0;
}

CLI_COMMAND(mv) {
  pH.measurepH();
  dev->print("mV: ");
  dev->print(pH.mV);
  return 0;
}

CLI_COMMAND(low) {
  if (argc == 2) {
    pH.calibrateProbeLow(atof(argv[1]));
  }

  dev->print("low reference / read: "); dev->print(pH.getCalibrateLow(), 2);
  dev->print(" / "); dev->print(pH.getCalibrateLowReading(), 2);
  return 0;
}

CLI_COMMAND(high) {
  if (argc == 2) {
    pH.calibrateProbeHigh(atof(argv[1]));
  }

  dev->print("high reference / read: "); dev->print(pH.getCalibrateHigh(), 2);
  dev->print(" / "); dev->print(pH.getCalibrateHighReading(), 2);
  return 0;
}

CLI_COMMAND(config) {
  dev->println("Config:");
  dev->print("  offset: "); dev->println(pH.getCalibrateOffset(), 2);
  dev->print("  dual point: "); dev->println(pH.usingDualPoint(), 2);
  dev->print("  low reference / read: "); dev->print(pH.getCalibrateLow(), 2);
  dev->print(" /  "); dev->println(pH.getCalibrateLowReading(), 2);
  dev->print("  high reference / read: "); dev->print(pH.getCalibrateHigh(), 2);
  dev->print(" / "); dev->println(pH.getCalibrateHighReading(), 2);
  dev->print("  temp. compensation: "); dev->println(pH.usingTemperatureCompensation());
  dev->print("    constant: "); dev->println(pH.getTempConstant());
  dev->print("  version: "); dev->print(pH.getVersion(), HEX);
  dev->println();
  dev->printPrompt();
  return 0;
}

CLI_COMMAND(tc) {
  if (argc == 2) {
    pH.useTemperatureCompensation(atof(argv[1]));
  }

  if (argc == 3) {
    pH.useTemperatureCompensation(atof(argv[1]));
    pH.setTempConstant(atof(argv[2]));
  }

  dev->print("temp. compensation: ");
  dev->println(pH.usingTemperatureCompensation());
  dev->print("   constant: ");
  dev->print(pH.getTempConstant());
  return 0;
}

CLI_COMMAND(dp) {
  if (argc == 2) {
    pH.useDualPoint(atof(argv[1]));
  }

  dev->print("dual point: ");
  dev->print(pH.usingDualPoint());
  return 0;
}

CLI_COMMAND(i2c) {
  if (argc == 2) {
    pH.setI2CAddress(atof(argv[1]));
  }

  return 0;
}
