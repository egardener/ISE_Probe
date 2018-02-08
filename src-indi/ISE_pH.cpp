#include "ISE_pH.h"

/*!
   \brief Class constructor
 */
ISE_pH::ISE_pH()
{
  _address = ISE_PROBE_I2C;
  Wire.begin();
}

ISE_pH::ISE_pH(uint8_t i2c_address)
{
  _address = i2c_address;
  Wire.begin();
}

/*!
   \brief Class destructor
 */
ISE_pH::~ISE_pH()
{}

/*!
   \code
    ISE_pH::measurepH();
   \endcode
   \brief Starts a pH measurement.
   \return pH
 */
float ISE_pH::measurepH()
{
  mV = measuremV();

  // Turn mV into pH
  pH  = fabs(7.0 - ((mV / 1000) / PROBE_MV_TO_PH));
  pOH = fabs(pH - 14);

  // Determine the temperature correction
  if (usingTemperatureCompensation())
  {
    float   temp             = measureTemp();
    uint8_t distance_from_7  = abs(7 - round(pH));
    uint8_t distance_from_25 = floor(abs(25 - round(temp)) / 10);
    float   temp_multiplier  = (distance_from_25 * distance_from_7) * TEMP_CORRECTION_FACTOR;

    if ((this->pH >= 8.0) && (temp >= 35))
    {
      // negative
      temp_multiplier *= -1;
    }

    if ((this->pH <= 6.0) && (temp <= 15))
    {
      // negative
      temp_multiplier *= -1;
    }

    this->pH += temp_multiplier;
  }

  if ((this->pH <= 0.0) || (this->pH > 14.0)) {
    this->pH  = -1;
    this->pOH = -1;
  }
  if (isinf(this->pH)) {
    this->pH  = -1;
    this->pOH = -1;
  }
  if (isnan(this->pH)) {
    this->pH  = -1;
    this->pOH = -1;
  }
  return pH;
}

float ISE_pH::measuremV()
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
float ISE_pH::measureTemp()
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
void ISE_pH::calibrateSingle(float solutionpH)
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
void ISE_pH::calibrateProbeLow(float solutionpH)
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
void ISE_pH::calibrateProbeHigh(float solutionpH)
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
void ISE_pH::setDualPointCalibration(float refLow,
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
float ISE_pH::getCalibrateOffset()
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
float ISE_pH::getCalibrateHighReference()
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
float ISE_pH::getCalibrateHighReading()
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
float ISE_pH::getCalibrateLowReference()
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
float ISE_pH::getCalibrateLowReading()
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
void ISE_pH::useTemperatureCompensation(bool b)
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
void ISE_pH::useDualPoint(bool b)
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
}

/*!
   \code
   uint8_t version = ISE_Probe::getVersion();
   \endcode
   \brief Retrieves the firmware version of the device
   \return   version of firmware
 */
uint8_t ISE_pH::getVersion()
{
  return _read_byte(ISE_VERSION_REGISTER);
}

/*!
   \brief Resets all the stored calibration information.
 */
void ISE_pH::reset()
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
bool ISE_pH::usingTemperatureCompensation()
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
bool ISE_pH::usingDualPoint()
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
void ISE_pH::setI2CAddress(uint8_t i2cAddress)
{
  _write_register(ISE_SOLUTION_REGISTER, i2cAddress);
  _send_command(ISE_I2C);
  _address = i2cAddress;
}

float ISE_pH::readEEPROM(uint8_t address)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _send_command(ISE_MEMORY_READ);
  return _read_register(ISE_BUFFER_REGISTER);
}

void ISE_pH::writeEEPROM(uint8_t address, float value)
{
  _write_register(ISE_SOLUTION_REGISTER, address);
  _write_register(ISE_BUFFER_REGISTER,   value);
  _send_command(ISE_MEMORY_WRITE);
}

void ISE_pH::_change_register(uint8_t r)
{
  Wire.beginTransmission(_address);
  Wire.write(r);
  Wire.endTransmission();
  delay(10);
}

void ISE_pH::_send_command(uint8_t command)
{
  Wire.beginTransmission(_address);
  Wire.write(ISE_TASK_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
  delay(10);
}

void ISE_pH::_write_register(uint8_t reg, float f)
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

float ISE_pH::_read_register(uint8_t reg)
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

void ISE_pH::_write_byte(uint8_t reg, uint8_t val)
{
  uint8_t b[5];

  b[0] = reg;
  b[1] = val;
  Wire.beginTransmission(_address);
  Wire.write(b, 2);
  Wire.endTransmission();
  delay(10);
}

uint8_t ISE_pH::_read_byte(uint8_t reg)
{
  uint8_t retval;

  _change_register(reg);
  Wire.requestFrom(_address, (uint8_t)1);
  retval = Wire.read();
  delay(10);
  return retval;
}
