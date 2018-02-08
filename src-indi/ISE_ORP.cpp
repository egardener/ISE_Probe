#include "ISE_ORP.h"

/*!
   \brief Class constructor
 */
ISE_ORP::ISE_ORP()
{
  _address = ISE_PROBE_I2C;
  Wire.begin();
}

ISE_ORP::ISE_ORP(uint8_t i2c_address)
{
  _address = i2c_address;
  Wire.begin();
}

/*!
   \brief Class destructor
 */
ISE_ORP::~ISE_ORP()
{}

/*!
   \code
    ISE_ORP::measureORP();
   \endcode
   \brief Starts an ORP measurement.
   \return ORP
 */
float ISE_ORP::measureORP()
{
  mV  = measuremV();
  ORP = mV;

  // this->Eh  = this->mV + getProbePotential();

  return mV;
}

void ISE_ORP::setProbePotential(uint32_t potential)
{
  writeEEPROM(POTENTIAL_REGISTER_ADDRESS, potential);
}

uint32_t ISE_ORP::getProbePotential()
{
  return readEEPROM(POTENTIAL_REGISTER_ADDRESS);
}

float ISE_ORP::measuremV()
{
  _send_command(ISE_MEASURE_MV);
  delay(ISE_MV_MEASURE_TIME);
  mV = _read_register(ISE_MV_REGISTER);
  Eh = mV + getProbePotential();

  if (isinf(mV)) {
    mV = NAN;
    Eh = NAN;
  }
  if (isnan(mV)) {
    mV = NAN;
    Eh = NAN;
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
float ISE_ORP::measureTemp()
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
void ISE_ORP::calibrateSingle(float solutionpH)
{
  _write_register(ISE_SOLUTION_REGISTER, solutionpH);
  _send_command(ISE_CALIBRATE_SINGLE);
  delay(ISE_MV_MEASURE_TIME);
}

/*!
   \code
   float z = ISE_Probe::getCalibrateOffset();
   \endcode
   \brief Retrieves the single-point offset from the device.
   \return   the probe's offset
 */
float ISE_ORP::getCalibrateOffset()
{
  return _read_register(ISE_CALIBRATE_SINGLE_REGISTER);
}

/*!
   \code
   uint8_t version = ISE_Probe::getVersion();
   \endcode
   \brief Retrieves the firmware version of the device
   \return   version of firmware
 */
uint8_t ISE_ORP::getVersion()
{
  return _read_byte(ISE_VERSION_REGISTER);
}

/*!
   \brief Resets all the stored calibration information.
 */
void ISE_ORP::reset()
{
  _write_register(ISE_CALIBRATE_SINGLE_REGISTER, NAN);
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
void ISE_ORP::setI2CAddress(uint8_t i2cAddress)
{
  _write_register(ISE_SOLUTION_REGISTER, i2cAddress);
  _send_command(ISE_I2C);
  _address = i2cAddress;
}

uint32_t ISE_ORP::readEEPROM(uint8_t address)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _send_command(ISE_MEMORY_READ);
  return _read_register(ISE_BUFFER_REGISTER);
}

void ISE_ORP::writeEEPROM(uint8_t address, uint32_t value)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _write_register(ISE_BUFFER_REGISTER,   value);
  _send_command(ISE_MEMORY_WRITE);
}

void ISE_ORP::_change_register(uint8_t r)
{
  Wire.beginTransmission(_address);
  Wire.write(r);
  Wire.endTransmission();
  delay(10);
}

void ISE_ORP::_send_command(uint8_t command)
{
  Wire.beginTransmission(_address);
  Wire.write(ISE_TASK_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
  delay(10);
}

void ISE_ORP::_write_register(uint8_t reg, float f)
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

float ISE_ORP::_read_register(uint8_t reg)
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

void ISE_ORP::_write_byte(uint8_t reg, uint8_t val)
{
  uint8_t b[5];

  b[0] = reg;
  b[1] = val;
  Wire.beginTransmission(_address);
  Wire.write(b, 2);
  Wire.endTransmission();
  delay(10);
}

uint8_t ISE_ORP::_read_byte(uint8_t reg)
{
  uint8_t retval;

  _change_register(reg);
  Wire.requestFrom(_address, (uint8_t)1);
  retval = Wire.read();
  delay(10);
  return retval;
}
