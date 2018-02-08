var C = {
  PH_PROBE: 0x3d,
  PH_MEASURE_PH: 80,
  PH_MEASURE_TEMP: 40,
  PH_CALIBRATE_SINGLE: 20,
  PH_CALIBRATE_LOW: 10,
  PH_CALIBRATE_HIGH: 8,
  PH_I2C: 1,
  PH_VERSION_REGISTER: 0,
  PH_PH_REGISTER: 1,
  PH_TEMP_REGISTER: 5,
  PH_CALIBRATE_SINGLE_REGISTER: 9,
  PH_CALIBRATE_REFHIGH_REGISTER: 13,
  PH_CALIBRATE_REFLOW_REGISTER: 17,
  PH_CALIBRATE_READHIGH_REGISTER: 21,
  PH_CALIBRATE_READLOW_REGISTER: 25,
  PH_SOLUTION_REGISTER: 29,
  PH_MV_REGISTER: 33,
  PH_CONFIG_REGISTER: 37,
  PH_TASK_REGISTER: 38,
  PH_TEMP_MEASURE_TIME: 750,
  PH_PH_MEASURE_TIME: 1100,
  PH_DUALPOINT_CONFIG_BIT: 0,
  PH_TEMP_COMPENSATION_CONFIG_BIT: 1
};

function pHProbe(_i2c, _address) {
  this.i2c = _i2c;
  this.address = _address;
}

pHProbe.prototype._send_command = function(cmd) {
  this.i2c.writeTo(this.address, [C.PH_TASK_REGISTER, cmd]);
  for (f = 0; f < 10000; f++) {}
};

pHProbe.prototype._read_byte = function(reg) {
  _change_register(reg);
  return this.i2c.readFrom(this.address, 1)[0];
  for (f = 0; f < 10000; f++) {}
};

pHProbe.prototype._write_byte = function(reg, val) {
  this.i2c.writeTo(this.address, [reg, val]);
  for (f = 0; f < 10000; f++) {}
};

pHProbe.prototype._change_register = function(reg) {
  this.i2c.writeTo(this.address, reg);
  for (f = 0; f < 10000; f++) {}
};

pHProbe.prototype._read_register = function(reg) {
  var data = new Uint8Array(4);

  this._change_register(reg);
  data[0] = this.i2c.readFrom(this.address, 1)[0];
  for (f = 0; f < 10000; f++) {}
  data[1] = this.i2c.readFrom(this.address, 1)[0];
  for (f = 0; f < 10000; f++) {}
  data[2] = this.i2c.readFrom(this.address, 1)[0];
  for (f = 0; f < 10000; f++) {}
  data[3] = this.i2c.readFrom(this.address, 1)[0];
  for (f = 0; f < 10000; f++) {}

  return new Float32Array(data.buffer)[0];
};

pHProbe.prototype._write_register = function(reg, val) {
  var data = new Float32Array(1);
  data[0] = val;

  this.i2c.writeTo(this.address, [reg, data.buffer[0], data.buffer[1], data.buffer[2], data.buffer[3]]);
  for (f = 0; f < 10000; f++) {}
};

exports.connect = function(_i2c, _address) {
  return new pHProbe(_i2c, _address);
};

pHProbe.prototype.measureTemp = function(callback) {
  var tempC;
  var tempF;
  var self = this;

  this._send_command(C.PH_MEASURE_TEMP);
  var i = setInterval(function() {
    tempC = self._read_register(C.PH_TEMP_REGISTER);
    tempF = ((tempC * 9) / 5) + 32;
    callback({
      tempC: tempC,
      tempF: tempF
    });
    clearInterval(i);
  }, C.PH_TEMP_MEASURE_TIME);
};

pHProbe.prototype.measurepH = function(callback) {
  var pH;
  var mV;
  var pOH;
  var self = this;

  if (this.useTemperatureCompensation == true) {
    this.measureTemp();
  }
  this._send_command(C.PH_MEASURE_PH);
  var i = setInterval(function() {
    pH = self._read_register(C.PH_PH_REGISTER);
    mV = self._read_register(C.PH_MV_REGISTER) * 1000;
    pH = Math.round(pH * 100.0) / 100.0;
    mV = Math.round(mV * 100.0) / 100.0;
    pOH = Math.abs(pH - 14);
    callback({
      pH: pH,
      mV: mV,
      pOH: pOH
    });
    clearInterval(i);
  }, C.PH_PH_MEASURE_TIME);
};

pHProbe.prototype.calibrateSingle = function(solutionpH, callback) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.PH_SOLUTION_REGISTER, solutionpH);
  this._send_command(C.PH_CALIBRATE_SINGLE);
  this.useDualPoint(dualpoint);
  var i = setInterval(function() {
    callback({
      offset: self._read_register(C.PH_CALIBRATE_SINGLE_REGISTER)
    });
    clearInterval(i);
  }, C.PH_PH_MEASURE_TIME);
};

pHProbe.prototype.calibrateLow = function(solutionpH) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.PH_SOLUTION_REGISTER, solutionpH);
  this._send_command(C.PH_CALIBRATE_LOW);
  this.useDualPoint(dualpoint);
};

pHProbe.prototype.calibrateHigh = function(solutionpH) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.PH_SOLUTION_REGISTER, solutionpH);
  this._send_command(C.PH_CALIBRATE_HIGH);
  //time.sleep(PH_PH_MEASURE_TIME / 1000.0)
  this.useDualPoint(dualpoint);
};

pHProbe.prototype.getVersion = function() {
  return this._read_byte(C.PH_VERSION_REGISTER);
};

pHProbe.prototype.getCalibrateOffset = function() {
  return Math.round(this._read_register(C.PH_CALIBRATE_SINGLE_REGISTER) * 100.0) / 100.0;
};

pHProbe.prototype.getCalibrateHigh = function() {
  return Math.round(this._read_register(C.PH_CALIBRATE_REFHIGH_REGISTER) * 100.0) / 100.0;;
};

pHProbe.prototype.getCalibrateLow = function() {
  return Math.round(this._read_register(C.PH_CALIBRATE_REFLOW_REGISTER) * 100.0) / 100.0;;
};

pHProbe.prototype.getCalibrateHighReading = function() {
  return Math.round(this._read_register(C.PH_CALIBRATE_READHIGH_REGISTER) * 100.0) / 100.0;;
};

pHProbe.prototype.getCalibrateLowReading = function() {
  return Math.round(this._read_register(C.PH_CALIBRATE_READLOW_REGISTER) * 100.0) / 100.0;;
};

pHProbe.prototype.reset = function() {
  this._write_register(C.PH_CALIBRATE_SINGLE_REGISTER, 1);
  this._write_register(C.PH_CALIBRATE_REFHIGH_REGISTER, 1);
  this._write_register(C.PH_CALIBRATE_REFLOW_REGISTER, 1);
  this._write_register(C.PH_CALIBRATE_READHIGH_REGISTER, 1);
  this._write_register(C.PH_CALIBRATE_READLOW_REGISTER, 1);
};

pHProbe.prototype.setDualPointCalibration = function(refLow, refHigh, readLow, readHigh) {
  this._write_register(C.PH_CALIBRATE_REFLOW_REGISTER, refLow);
  this._write_register(C.PH_CALIBRATE_REFHIGH_REGISTER, refHigh);
  this._write_register(C.PH_CALIBRATE_READLOW_REGISTER, readLow);
  this._write_register(C.PH_CALIBRATE_READHIGH_REGISTER, readHigh);
};

pHProbe.prototype.setI2CAddress = function(i2cAddress) {
  this._write_byte(C.PH_SOLUTION_REGISTER, i2cAddress);
  this._send_command(C.PH_I2C);
  this.address = i2cAddress;
};

pHProbe.prototype.useTemperatureCompensation = function(b) {
  var retval = this._read_byte(C.PH_CONFIG_REGISTER);

  retval = this._bit_set(retval, C.PH_TEMP_COMPENSATION_CONFIG_BIT, b);
  this._write_byte(C.PH_CONFIG_REGISTER, retval);
};

pHProbe.prototype.useDualPoint = function(b) {
  var retval = this._read_byte(C.PH_CONFIG_REGISTER);

  retval = this._bit_set(retval, C.PH_DUALPOINT_CONFIG_BIT, b);
  this._write_byte(C.PH_CONFIG_REGISTER, retval);
};

pHProbe.prototype.usingTemperatureCompensation = function() {
  var retval = this._read_byte(C.PH_CONFIG_REGISTER);
  return (retval >> 1) & 0x01;
};

pHProbe.prototype.usingDualPoint = function() {
  var retval = this._read_byte(C.PH_CONFIG_REGISTER);
  return (retval >> 0) & 0x01;
};

pHProbe.prototype._bit_set = function(v, index, x) {
  var mask = 1 << index;
  v &= ~mask;
  if (x) {
    v |= mask;
  }
  return v;
};