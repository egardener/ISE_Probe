/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */
 #include "uFire_ISE.h"

 #include "CLI.h"

/*
   This allows you to run various functions on a command-line like interface.
   For more information and documentation, visit ufire.co

   Display config information
    config

   Basic usage:
    mv

   Enable temperature compensation
    tc 1
    temp

   Single Point Calibration
    cal 200

   Dual Point Calibration
    low 50
    high 200
    dp 1

   Change the I2C address
    i2c 30

 */

CLI_COMMAND(reset);  // 'reset' reset all calibration configuration
CLI_COMMAND(temp);   // 'temp' measure temperature
CLI_COMMAND(cal);    // 'cal 500' calibrate the offset of the probe from mV 500
CLI_COMMAND(mv);     // 'mv' starts a measurement and returns the mV on the probe
CLI_COMMAND(low);    // 'low 50' calibrates the low dual-point calibration settings using low solution
CLI_COMMAND(high);   // 'high 200'calibrates the high dual-point calibration settings using high solution
CLI_COMMAND(config); // 'config' shows configuration information
CLI_COMMAND(tc);     // 'tc 0/1' temperature compensation; first argument to use it, second the constant to use
CLI_COMMAND(dp);     // 'dp 0/1' to use dual-point calibration or not
CLI_COMMAND(i2c);    // 'ic2 48' changes the i2c address to 48
CLI_COMMAND(data);   // 'data' shows the data registers
CLI_COMMAND(read);   // 'read' read a float/long/4 bytes from EEPROM address 100
CLI_COMMAND(write);  // 'write 200' to EEPROM address 100

ISE_Probe mV;

void setup()
{
  Serial.begin(9600);
  delay(1000); Serial.println();
  CLI.setDefaultPrompt("> ");
  CLI.onConnect(config);

  CLI.addCommand("reset",  reset);
  CLI.addCommand("temp",   temp);
  CLI.addCommand("cal",    cal);
  CLI.addCommand("mv",     mv);
  CLI.addCommand("low",    low);
  CLI.addCommand("high",   high);
  CLI.addCommand("config", config);
  CLI.addCommand("tc",     tc);
  CLI.addCommand("dp",     dp);
  CLI.addCommand("i2c",    i2c);
  CLI.addCommand("data",   data);
  CLI.addCommand("read",   read);
  CLI.addCommand("write",  write);

  CLI.addClient(Serial);
}

void loop()
{
  CLI.process();
}

CLI_COMMAND(reset) {
  mV.reset();
  return 0;
}

CLI_COMMAND(temp) {
  mV.measureTemp();
  dev->print("C/F: "); dev->print(mV.tempC);
  dev->print(" /  "); dev->println(mV.tempF);
  return 0;
}

CLI_COMMAND(cal) {
  if (argc == 2) {
    mV.calibrateSingle(atof(argv[1]));
  }
  dev->print("offset: ");
  dev->print(mV.getCalibrateOffset());
  return 0;
}

CLI_COMMAND(mv) {
  for (;;) {
    mV.measuremV();
    dev->print("mV: ");
    dev->print(mV.mV, 4);

    dev->println();
    delay(2500);
  }
  return 0;
}

CLI_COMMAND(data) {
  dev->print("mV: ");
  dev->println(mV.mV);
  dev->print("C/F: "); dev->print(mV.tempC);
  dev->print(" /  "); dev->println(mV.tempF);
  return 0;
}
CLI_COMMAND(low) {
  if (argc == 2) {
    mV.calibrateProbeLow(atof(argv[1]));
  }

  dev->print("low reference / read: "); dev->print(mV.getCalibrateLowReference(), 2);
  dev->print(" / "); dev->print(mV.getCalibrateLowReading(), 2);
  return 0;
}

CLI_COMMAND(high) {
  if (argc == 2) {
    mV.calibrateProbeHigh(atof(argv[1]));
  }

  dev->print("high reference / read: "); dev->print(mV.getCalibrateHighReference(), 2);
  dev->print(" / "); dev->print(mV.getCalibrateHighReading(), 2);
  return 0;
}

CLI_COMMAND(config) {
  dev->println("Config:");
  dev->print("  offset: "); dev->println(mV.getCalibrateOffset(), 2);
  dev->print("  dual point: "); dev->println(mV.usingDualPoint(), 2);
  dev->print("  low reference / read: "); dev->print(mV.getCalibrateLowReference(), 2);
  dev->print(" / "); dev->println(mV.getCalibrateLowReading(), 2);
  dev->print("  high reference / read: "); dev->print(mV.getCalibrateHighReference(), 2);
  dev->print(" / "); dev->println(mV.getCalibrateHighReading(), 2);
  dev->print("  temp. compensation: "); dev->println(mV.usingTemperatureCompensation());
  dev->print("  version: "); dev->print(mV.getVersion(), HEX);
  dev->println();
  dev->printPrompt();
  return 0;
}

CLI_COMMAND(tc) {
  if (argc == 2) {
    mV.useTemperatureCompensation(atof(argv[1]));
  }

  dev->print("temp. compensation: ");
  dev->println(mV.usingTemperatureCompensation());
  return 0;
}

CLI_COMMAND(dp) {
  if (argc == 2) {
    mV.useDualPoint(atof(argv[1]));
  }

  dev->print("dual point: ");
  dev->print(mV.usingDualPoint());
  return 0;
}

CLI_COMMAND(i2c) {
  if (argc == 2) {
    mV.setI2CAddress(atof(argv[1]));
  }

  return 0;
}

CLI_COMMAND(read) {
  dev->print(mV.readEEPROM(100));
  return 0;
}

CLI_COMMAND(write) {
  if (argc == 2) {
    mV.writeEEPROM(100, atof(argv[1]));
  }

  return 0;
}
