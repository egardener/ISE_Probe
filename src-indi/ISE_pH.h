#ifndef ISE_PH_H
#define ISE_PH_H

#include <math.h>

#if defined(PARTICLE)
# include "application.h"
# define bitRead(value, bit) (((value) >> (bit)) & 0x01)
# define bitSet(value, bit) ((value) |= (1UL << (bit)))
# define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
# define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#else // if defined(PARTICLE)
# include <Arduino.h>
# include <Wire.h>
#endif // if defined(PARTICLE)

#define ISE_PROBE_I2C 0x3F

#define ISE_MEASURE_MV 80
#define ISE_MEASURE_TEMP 40
#define ISE_CALIBRATE_SINGLE 20
#define ISE_CALIBRATE_LOW 10
#define ISE_CALIBRATE_HIGH 8
#define ISE_MEMORY_WRITE 4
#define ISE_MEMORY_READ 2
#define ISE_I2C 1

#define ISE_VERSION_REGISTER 0             /*!< version */
#define ISE_MV_REGISTER 1                  /*!< pH */
#define ISE_TEMP_REGISTER 5                /*!< temperature in C */
#define ISE_CALIBRATE_SINGLE_REGISTER 9    /*!< calibration offset */
#define ISE_CALIBRATE_REFHIGH_REGISTER 13  /*!< reference high calibration */
#define ISE_CALIBRATE_REFLOW_REGISTER 17   /*!< reference low calibration */
#define ISE_CALIBRATE_READHIGH_REGISTER 21 /*!< reading high calibration */
#define ISE_CALIBRATE_READLOW_REGISTER 25  /*!< reading low calibration */
#define ISE_SOLUTION_REGISTER  29          /*!< reference ISE solution */
#define ISE_BUFFER_REGISTER 33             /*!< measured mV from pH probe */
#define ISE_CONFIG_REGISTER 37             /*!< config register */
#define ISE_TASK_REGISTER 38               /*!< task register */

#define ISE_TEMP_MEASURE_TIME 750
#define ISE_MV_MEASURE_TIME 1500

#define ISE_DUALPOINT_CONFIG_BIT 0         /*!< dual point config bit */
#define ISE_TEMP_COMPENSATION_CONFIG_BIT 1 /*!< temperature compensation config bit */
#define PROBE_MV_TO_PH 0.0592
#define TEMP_CORRECTION_FACTOR 0.03

class ISE_pH {
public:

  float pH;    /*!< pH */
  float pOH;   /*!< pOH */
  float tempC; /*!< Temperature in C */
  float tempF; /*!< Temperature in F */
  float mV;    /*!< mV of probe */
  ISE_pH(uint8_t i2c_address);
  ISE_pH();
  ~ISE_pH();
  float measurepH();
  float measuremV();
  float measureTemp();
  void  calibrateSingle(float solutionpH);
  void  calibrateProbeLow(float solutionpH);
  void  calibrateProbeHigh(float solutionpH);
  void  setDualPointCalibration(float refLow,
                                float refHigh,
                                float readLow,
                                float readHigh);
  float   getCalibrateOffset();
  void    useTemperatureCompensation(bool b);
  bool    usingTemperatureCompensation();
  void    useDualPoint(bool b);
  bool    usingDualPoint();
  float   getCalibrateHighReference();
  float   getCalibrateLowReference();
  float   getCalibrateHighReading();
  float   getCalibrateLowReading();
  uint8_t getVersion();
  void    reset();
  void    setI2CAddress(uint8_t i2cAddress);
  void    writeEEPROM(uint8_t address,
                      float   value);
  float   readEEPROM(uint8_t address);

private:

  uint8_t _address;
  void    _change_register(uint8_t register);
  void    _send_command(uint8_t command);
  void    _write_register(uint8_t reg,
                          float   f);
  void    _write_byte(uint8_t reg,
                      uint8_t val);
  float   _read_register(uint8_t reg);
  uint8_t _read_byte(uint8_t reg);
};

  #endif // ifndef ISEPROBE_H
