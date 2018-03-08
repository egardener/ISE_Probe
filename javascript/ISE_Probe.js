var C = {
  ISE_PROBE: 0x3f,
  ISE_MEASURE_MV: 80,
  ISE_MEASURE_TEMP: 40,
  ISE_CALIBRATE_SINGLE: 20,
  ISE_CALIBRATE_LOW: 10,
  ISE_CALIBRATE_HIGH: 8,
  ISE_MEMORY_WRITE: 4,
  ISE_MEMORY_READ: 2,
  ISE_I2C: 1,
  ISE_VERSION_REGISTER: 0,
  ISE_MV_REGISTER: 1,
  ISE_TEMP_REGISTER: 5,
  ISE_CALIBRATE_SINGLE_REGISTER: 9,
  ISE_CALIBRATE_REFHIGH_REGISTER: 13,
  ISE_CALIBRATE_REFLOW_REGISTER: 17,
  ISE_CALIBRATE_READHIGH_REGISTER: 21,
  ISE_CALIBRATE_READLOW_REGISTER: 25,
  ISE_SOLUTION_REGISTER: 29,
  ISE_BUFFER_REGISTER: 33,
  ISE_CONFIG_REGISTER: 37,
  ISE_TASK_REGISTER: 38,
  ISE_TEMP_MEASURE_TIME: 750,
  ISE_MV_MEASURE_TIME: 1500,
  ISE_DUALPOINT_CONFIG_BIT: 0,
  ISE_TEMP_COMPENSATION_CONFIG_BIT: 1
};

function ISE_Probe(_i2c, _address) {
  this.i2c = _i2c;
  this.address = _address;
}

ISE_Probe.prototype._send_command = function(cmd) {
  this.i2c.writeTo(this.address, [C.ISE_TASK_REGISTER, cmd]);
};

ISE_Probe.prototype._read_byte = function(reg) {
  this._change_register(reg);
  return this.i2c.readFrom(this.address, 1)[0];
};

ISE_Probe.prototype._write_byte = function(reg, val) {
  this.i2c.writeTo(this.address, [reg, val]);
};

ISE_Probe.prototype._change_register = function(reg) {
  this.i2c.writeTo(this.address, reg);
};

ISE_Probe.prototype._read_register = function(reg) {
  var data = new Uint8Array(4);

  this._change_register(reg);
  data[0] = this.i2c.readFrom(this.address, 1)[0];
  data[1] = this.i2c.readFrom(this.address, 1)[0];
  data[2] = this.i2c.readFrom(this.address, 1)[0];
  data[3] = this.i2c.readFrom(this.address, 1)[0];

  return new Float32Array(data.buffer)[0];
};

ISE_Probe.prototype._write_register = function(reg, val) {
  var data = new Float32Array(1);
  data[0] = val;

  this.i2c.writeTo(this.address, [reg, data.buffer[0], data.buffer[1], data.buffer[2], data.buffer[3]]);
};

exports.connect = function(_i2c, _address) {
  return new ISE_Probe(_i2c, _address);
};

ISE_Probe.prototype.measureTemp = function(callback) {
  var tempC;
  var tempF;
  var self = this;

  this._send_command(C.ISE_MEASURE_TEMP);
  var i = setInterval(function() {
    tempC = self._read_register(C.ISE_TEMP_REGISTER);
    tempF = ((tempC * 9) / 5) + 32;
    callback({
      tempC: tempC,
      tempF: tempF
    });
    clearInterval(i);
  }, C.ISE_TEMP_MEASURE_TIME);
};

ISE_Probe.prototype.measuremV = function(callback) {
  var mV;
  var self = this;

  this._send_command(C.ISE_MEASURE_MV);
  var i = setInterval(function() {
    mV = self._read_register(C.ISE_MV_REGISTER);
    callback({
      mV: mV,
    });
    clearInterval(i);
  }, C.ISE_MV_MEASURE_TIME);
};

ISE_Probe.prototype.calibrateSingle = function(solutionmV, callback) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.ISE_SOLUTION_REGISTER, solutionmV);
  this._send_command(C.ISE_CALIBRATE_SINGLE);
  this.useDualPoint(dualpoint);
  var i = setInterval(function() {
    callback({});
    clearInterval(i);
  }, C.ISE_MV_MEASURE_TIME);
};

ISE_Probe.prototype.calibrateProbeLow = function(solutionmV, callback) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.ISE_SOLUTION_REGISTER, solutionmV);
  this._send_command(C.ISE_CALIBRATE_LOW);
  this.useDualPoint(dualpoint);
  var i = setInterval(function() {
    callback({});
    clearInterval(i);
  }, C.ISE_MV_MEASURE_TIME);
};

ISE_Probe.prototype.calibrateProbeHigh = function(solutionmV, callback) {
  var dualpoint = this.usingDualPoint();
  var self = this;

  this.useDualPoint(false);
  this._write_register(C.ISE_SOLUTION_REGISTER, solutionmV);
  this._send_command(C.ISE_CALIBRATE_HIGH);
  this.useDualPoint(dualpoint);
  var i = setInterval(function() {
    callback({});
    clearInterval(i);
  }, C.ISE_MV_MEASURE_TIME);
};

ISE_Probe.prototype.getVersion = function() {
  return this._read_byte(C.ISE_VERSION_REGISTER);
};

ISE_Probe.prototype.getCalibrateOffset = function() {
  return Math.round(this._read_register(C.ISE_CALIBRATE_SINGLE_REGISTER) * 100.0) / 100.0;
};

ISE_Probe.prototype.getCalibrateHighReference = function() {
  return Math.round(this._read_register(C.ISE_CALIBRATE_REFHIGH_REGISTER) * 100.0) / 100.0;;
};

ISE_Probe.prototype.getCalibrateLowReference = function() {
  return Math.round(this._read_register(C.ISE_CALIBRATE_REFLOW_REGISTER) * 100.0) / 100.0;;
};

ISE_Probe.prototype.getCalibrateHighReading = function() {
  return Math.round(this._read_register(C.ISE_CALIBRATE_READHIGH_REGISTER) * 100.0) / 100.0;;
};

ISE_Probe.prototype.getCalibrateLowReading = function() {
  return Math.round(this._read_register(C.ISE_CALIBRATE_READLOW_REGISTER) * 100.0) / 100.0;;
};

ISE_Probe.prototype.reset = function() {
  this._write_register(C.ISE_CALIBRATE_SINGLE_REGISTER, NaN);
  this._write_register(C.ISE_CALIBRATE_REFHIGH_REGISTER, NaN);
  this._write_register(C.ISE_CALIBRATE_REFLOW_REGISTER, NaN);
  this._write_register(C.ISE_CALIBRATE_READHIGH_REGISTER, NaN);
  this._write_register(C.ISE_CALIBRATE_READLOW_REGISTER, NaN);
};

ISE_Probe.prototype.setDualPointCalibration = function(refLow, refHigh, readLow, readHigh) {
  this._write_register(C.ISE_CALIBRATE_REFLOW_REGISTER, refLow);
  this._write_register(C.ISE_CALIBRATE_REFHIGH_REGISTER, refHigh);
  this._write_register(C.ISE_CALIBRATE_READLOW_REGISTER, readLow);
  this._write_register(C.ISE_CALIBRATE_READHIGH_REGISTER, readHigh);
};

ISE_Probe.prototype.setI2CAddress = function(i2cAddress) {
  this._write_byte(C.ISE_SOLUTION_REGISTER, i2cAddress);
  this._send_command(C.ISE_I2C);
  this.address = i2cAddress;
};

ISE_Probe.prototype.useTemperatureCompensation = function(b) {
  var retval = this._read_byte(C.ISE_CONFIG_REGISTER);

  retval = this._bit_set(retval, C.ISE_TEMP_COMPENSATION_CONFIG_BIT, b);
  this._write_byte(C.ISE_CONFIG_REGISTER, retval);
};

ISE_Probe.prototype.useDualPoint = function(b) {
  var retval = this._read_byte(C.ISE_CONFIG_REGISTER);

  retval = this._bit_set(retval, C.ISE_DUALPOINT_CONFIG_BIT, b);
  this._write_byte(C.ISE_CONFIG_REGISTER, retval);
};

ISE_Probe.prototype.usingTemperatureCompensation = function() {
  var retval = this._read_byte(C.ISE_CONFIG_REGISTER);
  return (retval >> 1) & 0x01;
};

ISE_Probe.prototype.usingDualPoint = function() {
  var retval = this._read_byte(C.ISE_CONFIG_REGISTER);
  return (retval >> 0) & 0x01;
};

ISE_Probe.prototype._bit_set = function(v, index, x) {
  var mask = 1 << index;
  v &= ~mask;
  if (x) {
    v |= mask;
  }
  return v;
};