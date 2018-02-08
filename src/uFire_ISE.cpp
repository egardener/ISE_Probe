// Copyright (c) 2018 Justin Decker

//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*!
   \file ISE_Probe.cpp
   \brief ISE Probe Class Implementation

   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire/ISE_Probe for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

#include <math.h>
#include "uFire_ISE.h"

/*!
   \brief Class constructor
 */
ISE_Probe::ISE_Probe()
{
  _address = ISE_PROBE_I2C;
  Wire.begin();
}

/*!
   \brief Class destructor
 */
ISE_Probe::~ISE_Probe()
{}

/*!
   \code
    ISE_Probe::measuremV();
   \endcode
   \brief Starts a measurement.
   \return mV
 */
float ISE_Probe::measuremV()
{
  _send_command(ISE_MEASURE_MV);
  delay(ISE_MV_MEASURE_TIME);
  mV = _read_register(ISE_MV_REGISTER);

  if (isinf(mV)) {
    mV = NAN;
  }
  if (isnan(mV)) {
    mV = NAN;
  }
  return mV;
}

/*!
   \code
    ISE_Probe::measureTemp();
   \endcode
   \brief Starts a temperature measurement
   \post #tempC and #tempF are updated
   \note A value of -127 means the device is not connected.
   \return temperature in C
 */
float ISE_Probe::measureTemp()
{
  _send_command(ISE_MEASURE_TEMP);
  delay(ISE_TEMP_MEASURE_TIME);
  tempC = _read_register(ISE_TEMP_REGISTER);
  tempF = ((tempC * 9) / 5) + 32;
  return tempC;
}

/*!
   \code
    ISE_Probe::calibrateSingle(7.0);
   \endcode
   \brief Calibrates the probe using a single point.
   \post result will be saved in the device's EEPROM and used
   automatically thereafter
 */
void ISE_Probe::calibrateSingle(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(ISE_SOLUTION_REGISTER, solutionpH);
  _send_command(ISE_CALIBRATE_SINGLE);
  delay(ISE_MV_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
    ISE_Probe::calibrateProbeLow(4.0);
   \endcode
   \brief Calibrates the dual-point values for the low reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void ISE_Probe::calibrateProbeLow(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(ISE_SOLUTION_REGISTER, solutionpH);
  _send_command(ISE_CALIBRATE_LOW);
  delay(ISE_MV_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
   ISE_Probe::calibrateProbeHigh(7.0);
   \endcode
   \brief Calibrates the dual-point values for the high reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void ISE_Probe::calibrateProbeHigh(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(ISE_SOLUTION_REGISTER, solutionpH);
  _send_command(ISE_CALIBRATE_HIGH);
  delay(ISE_MV_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
   ISE_Probe::setDualPointCalibration(4.0, 7.0, 3.8, 7.2);
   \endcode
   \brief Sets all the values for dual point calibration and saves them
   in the devices's EEPROM.
   \param refLow            the reference low point
   \param refHigh           the reference high point
   \param readLow           the measured low point
   \param readHigh          the measured high point
 */
void ISE_Probe::setDualPointCalibration(float refLow,
                                        float refHigh,
                                        float readLow,
                                        float readHigh)
{
  _write_register(ISE_CALIBRATE_REFLOW_REGISTER,   refLow);
  _write_register(ISE_CALIBRATE_REFHIGH_REGISTER,  refHigh);
  _write_register(ISE_CALIBRATE_READLOW_REGISTER,  readLow);
  _write_register(ISE_CALIBRATE_READHIGH_REGISTER, readHigh);
}

/*!
   \code
   float z = ISE_Probe::getCalibrateOffset();
   \endcode
   \brief Retrieves the single-point offset from the device.
   \return   the probe's offset
 */
float ISE_Probe::getCalibrateOffset()
{
  return _read_register(ISE_CALIBRATE_SINGLE_REGISTER);
}

/*!
   \code
   float calHigh = ISE_Probe::getCalibrateHigh();
   \endcode
   \brief Retrieves the reference-high calibration data
   \return   the reference-high
 */
float ISE_Probe::getCalibrateHighReference()
{
  return _read_register(ISE_CALIBRATE_REFHIGH_REGISTER);
}

/*!
   \code
   float calHigh = ISE_Probe::getCalibrateHighReading();
   \endcode
   \brief Retrieves the reading-high calibration data
   \return   the reading-high
 */
float ISE_Probe::getCalibrateHighReading()
{
  return _read_register(ISE_CALIBRATE_READHIGH_REGISTER);
}

/*!
   \code
   float calLow = ISE_Probe::getCalibrateLow();
   \endcode
   \brief Retrieves the reference-low calibration data
   \return   the reference-low
 */
float ISE_Probe::getCalibrateLowReference()
{
  return _read_register(ISE_CALIBRATE_REFLOW_REGISTER);
}

/*!
   \code
   float calLow = ISE_Probe::getCalibrateLowReading();
   \endcode
   \brief Retrieves the reference-low calibration data
   \return   the reading-low
 */
float ISE_Probe::getCalibrateLowReading()
{
  return _read_register(ISE_CALIBRATE_READLOW_REGISTER);
}

/*!
   \code
   ISE_Probe::useTemperatureCompensation(true);
   \endcode
   \brief Configures device to use temperature compensation or not
   \param b   true for false
 */
void ISE_Probe::useTemperatureCompensation(bool b)
{
  uint8_t retval;
  uint8_t config = _read_byte(ISE_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(config, ISE_TEMP_COMPENSATION_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(config, ISE_TEMP_COMPENSATION_CONFIG_BIT);
  }
  _write_byte(ISE_CONFIG_REGISTER, retval);
}

/*!
   \code
   ISE_Probe::useDualPoint(true);
   \endcode
   \brief Configures device to use dual-point calibration
   \param b   true or false
 */
void ISE_Probe::useDualPoint(bool b)
{
  uint8_t retval;
  uint8_t config = _read_byte(ISE_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(config, ISE_DUALPOINT_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(config, ISE_DUALPOINT_CONFIG_BIT);
  }

  _write_byte(ISE_CONFIG_REGISTER, retval);
  Serial.println(retval);
}

/*!
   \code
   uint8_t version = ISE_Probe::getVersion();
   \endcode
   \brief Retrieves the firmware version of the device
   \return   version of firmware
 */
uint8_t ISE_Probe::getVersion()
{
  return _read_byte(ISE_VERSION_REGISTER);
}

/*!
   \brief Resets all the stored calibration information.
 */
void ISE_Probe::reset()
{
  _write_register(ISE_CALIBRATE_SINGLE_REGISTER,   NAN);
  _write_register(ISE_CALIBRATE_REFHIGH_REGISTER,  NAN);
  _write_register(ISE_CALIBRATE_REFLOW_REGISTER,   NAN);
  _write_register(ISE_CALIBRATE_READHIGH_REGISTER, NAN);
  _write_register(ISE_CALIBRATE_READLOW_REGISTER,  NAN);
}

/*!
   \code
   bool usingTemp = ISE_Probe::usingTemperatureCompensation();
   \endcode
   \brief Boolean if temperature compensation is being used
   \return   true if using compensation, false otherwise
 */
bool ISE_Probe::usingTemperatureCompensation()
{
  uint8_t retval = _read_byte(ISE_CONFIG_REGISTER);

  return (retval >> ISE_TEMP_COMPENSATION_CONFIG_BIT)  & 0x01;
}

/*!
   \code
   bool usingTemp = ISE_Probe::usingDualPoint();
   \endcode
   \brief Boolean if dual point calibration is being used
   \return   true if using compensation, false otherwise
 */
bool ISE_Probe::usingDualPoint()
{
  uint8_t retval = _read_byte(ISE_CONFIG_REGISTER);

  return (retval >> ISE_DUALPOINT_CONFIG_BIT)  & 0x01;
}

/*!
   \code
    ISE_Probe::setI2CAddress(0x3d);
   \endcode

   \brief Changes the i2c address of the device.

   If the default address of the device needs to be changed, call this function to
   permanently change the address. If you forget the i2c address, you will need
   to use an i2c scanner to recover it.
 */
void ISE_Probe::setI2CAddress(uint8_t i2cAddress)
{
  _write_register(ISE_SOLUTION_REGISTER, i2cAddress);
  _send_command(ISE_I2C);
  _address = i2cAddress;
}

uint32_t ISE_Probe::readEEPROM(uint8_t address)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _send_command(ISE_MEMORY_READ);
  return _read_register(ISE_BUFFER_REGISTER);
}

void ISE_Probe::writeEEPROM(uint8_t address, uint32_t value)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _write_register(ISE_BUFFER_REGISTER,   value);
  _send_command(ISE_MEMORY_WRITE);
}

void ISE_Probe::_change_register(uint8_t r)
{
  Wire.beginTransmission(_address);
  Wire.write(r);
  Wire.endTransmission();
  delay(10);
}

void ISE_Probe::_send_command(uint8_t command)
{
  Wire.beginTransmission(_address);
  Wire.write(ISE_TASK_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
  delay(10);
}

void ISE_Probe::_write_register(uint8_t reg, float f)
{
  uint8_t b[5];
  float   f_val = f;

  b[0] = reg;
  b[1] = *((uint8_t *)&f_val);
  b[2] = *((uint8_t *)&f_val + 1);
  b[3] = *((uint8_t *)&f_val + 2);
  b[4] = *((uint8_t *)&f_val + 3);
  Wire.beginTransmission(_address);
  Wire.write(b, 5);
  Wire.endTransmission();
  delay(10);
}

float ISE_Probe::_read_register(uint8_t reg)
{
  float retval;

  _change_register(reg);

  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 1) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 2) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 3) = Wire.read();
  delay(10);
  return retval;
}

void ISE_Probe::_write_byte(uint8_t reg, uint8_t val)
{
  uint8_t b[5];

  b[0] = reg;
  b[1] = val;
  Wire.beginTransmission(_address);
  Wire.write(b, 2);
  Wire.endTransmission();
  delay(10);
}

uint8_t ISE_Probe::_read_byte(uint8_t reg)
{
  uint8_t retval;

  _change_register(reg);
  Wire.requestFrom(_address, (uint8_t)1);
  retval = Wire.read();
  delay(10);
  return retval;
}
