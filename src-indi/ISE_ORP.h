#ifndef ISE_ORP_H
#define ISE_ORP_H

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
#define ISE_MEMORY_WRITE 4
#define ISE_MEMORY_READ 2
#define ISE_I2C 1

#define ISE_VERSION_REGISTER 0          /*!< version */
#define ISE_MV_REGISTER 1               /*!< pH */
#define ISE_TEMP_REGISTER 5             /*!< temperature in C */
#define ISE_CALIBRATE_SINGLE_REGISTER 9 /*!< calibration offset */
#define ISE_SOLUTION_REGISTER  29       /*!< reference ISE solution */
#define ISE_BUFFER_REGISTER 33          /*!< measured mV from pH probe */
#define ISE_CONFIG_REGISTER 37          /*!< config register */
#define ISE_TASK_REGISTER 38            /*!< task register */

#define ISE_TEMP_MEASURE_TIME 750
#define ISE_MV_MEASURE_TIME 1500

#define ISE_DUALPOINT_CONFIG_BIT 0         /*!< dual point config bit */
#define ISE_TEMP_COMPENSATION_CONFIG_BIT 1 /*!< temperature compensation config bit */

#define POTENTIAL_REGISTER_ADDRESS 100

class ISE_ORP {
public:

  float ORP;   /*!< pH */
  float Eh;    /*!< pOH */
  float mV;
  float tempC; /*!< Temperature in C */
  float tempF; /*!< Temperature in F */
  ISE_ORP(uint8_t i2c_address);
  ISE_ORP();
  ~ISE_ORP();
  float    measureORP();
  void     setProbePotential(uint32_t potential);
  uint32_t getProbePotential();
  float    measuremV();
  float    measureTemp();
  void     calibrateSingle(float solutionpH);
  float    getCalibrateOffset();
  uint8_t  getVersion();
  void     reset();
  void     setI2CAddress(uint8_t i2cAddress);
  void     writeEEPROM(uint8_t  address,
                       uint32_t value);
  uint32_t readEEPROM(uint8_t address);

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

#endif // ifndef ISE_PH_H
