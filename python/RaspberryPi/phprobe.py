import math
import struct
import time
import smbus
global i2c

PH_PROBE = 0x3d

PH_MEASURE_PH = 80
PH_MEASURE_TEMP = 40
PH_CALIBRATE_SINGLE = 20
PH_CALIBRATE_LOW = 10
PH_CALIBRATE_HIGH = 8
PH_I2C = 1

PH_VERSION_REGISTER = 0
PH_PH_REGISTER = 1
PH_TEMP_REGISTER = 5
PH_CALIBRATE_SINGLE_REGISTER = 9
PH_CALIBRATE_REFHIGH_REGISTER = 13
PH_CALIBRATE_REFLOW_REGISTER = 17
PH_CALIBRATE_READHIGH_REGISTER = 21
PH_CALIBRATE_READLOW_REGISTER = 25
PH_SOLUTION_REGISTER = 29
PH_MV_REGISTER = 33
PH_CONFIG_REGISTER = 37
PH_TASK_REGISTER = 38

PH_TEMP_MEASURE_TIME = 750
PH_PH_MEASURE_TIME = 1100

PH_DUALPOINT_CONFIG_BIT = 0
PH_TEMP_COMPENSATION_CONFIG_BIT = 1


class phprobe(object):
    pH = 0
    pOH = 0
    mV = 0
    tempC = 0
    tempF = 0
    address = PH_PROBE

    def __init__(self, i2c_bus, **kwargs):
        global i2c
        i2c = smbus.SMBus(i2c_bus)

    def measureTemp(self):
        self._send_command(PH_MEASURE_TEMP)
        time.sleep(PH_TEMP_MEASURE_TIME / 1000.0)
        self.tempC = self._read_register(PH_TEMP_REGISTER)
        self.tempF = ((self.tempC * 9) / 5) + 32
        return self.tempC

    def measurepH(self):
        if self.usingTemperatureCompensation() is True:
            self.measureTemp()
        self._send_command(PH_MEASURE_PH)
        time.sleep(PH_PH_MEASURE_TIME / 1000.0)
        self.pH = self._read_register(PH_PH_REGISTER)
        self.mV = self._read_register(PH_MV_REGISTER) * 1000
        self.pH = round(self.pH * 100.0) / 100.0
        self.mV = round(self.mV * 100.0) / 100.0
        self.pOH = abs(self.pH - 14)

        if self.pH <= 0.0 or self.pH >= 14.0:
            self.pH = -1
            self.pOH = -1

        if math.isinf(self.pH):
            self.pH = -1
            self.pOH = -1

        if math.isnan(self.pH):
            self.pH = -1
            self.pOH = -1

        return self.pH

    def calibrateSingle(self, solutionpH):
        dualpoint = self.usingDualPoint()

        self.useDualPoint(0)
        self._write_register(PH_SOLUTION_REGISTER, solutionpH)
        self._send_command(PH_CALIBRATE_SINGLE)
        time.sleep(PH_PH_MEASURE_TIME / 1000.0)
        self.useDualPoint(dualpoint)

    def calibrateProbeLow(self, solutionpH):
        dualpoint = self.usingDualPoint()

        self.useDualPoint(0)
        self._write_register(PH_SOLUTION_REGISTER, solutionpH)
        self._send_command(PH_CALIBRATE_LOW)
        time.sleep(PH_PH_MEASURE_TIME / 1000.0)
        self.useDualPoint(dualpoint)

    def calibrateProbeHigh(self, solutionpH):
        dualpoint = self.usingDualPoint()

        self.useDualPoint(0)
        self._write_register(PH_SOLUTION_REGISTER, solutionpH)
        self._send_command(PH_CALIBRATE_HIGH)
        time.sleep(PH_PH_MEASURE_TIME / 1000.0)
        self.useDualPoint(dualpoint)

    def getVersion(self):
        return self._read_byte(PH_VERSION_REGISTER)

    def getCalibrateOffset(self):
        return self._read_register(PH_CALIBRATE_SINGLE_REGISTER)

    def getCalibrateHigh(self):
        return self._read_register(PH_CALIBRATE_REFHIGH_REGISTER)

    def getCalibrateLow(self):
        return self._read_register(PH_CALIBRATE_REFLOW_REGISTER)

    def getCalibrateHighReading(self):
        return self._read_register(PH_CALIBRATE_READHIGH_REGISTER)

    def getCalibrateLowReading(self):
        return self._read_register(PH_CALIBRATE_READLOW_REGISTER)

    def reset(self):
        n = float('nan')
        self._write_register(PH_CALIBRATE_SINGLE_REGISTER,   n)
        self._write_register(PH_CALIBRATE_REFHIGH_REGISTER,  n)
        self._write_register(PH_CALIBRATE_REFLOW_REGISTER,   n)
        self._write_register(PH_CALIBRATE_READHIGH_REGISTER, n)
        self._write_register(PH_CALIBRATE_READLOW_REGISTER,  n)

    def setDualPointCalibration(self, refLow, refHigh, readLow, readHigh):
        self._write_register(PH_CALIBRATE_REFLOW_REGISTER,   refLow)
        self._write_register(PH_CALIBRATE_REFHIGH_REGISTER,  refHigh)
        self._write_register(PH_CALIBRATE_READLOW_REGISTER,  readLow)
        self._write_register(PH_CALIBRATE_READHIGH_REGISTER, readHigh)

    def setI2CAddress(self, i2cAddress):
        self._write_byte(PH_SOLUTION_REGISTER, int(i2cAddress))
        self._send_command(PH_I2C)
        self.address = int(i2cAddress)

    def useTemperatureCompensation(self, b):
        retval = self._read_byte(PH_CONFIG_REGISTER)

        retval = self._bit_set(retval, PH_TEMP_COMPENSATION_CONFIG_BIT, b)
        self._write_byte(PH_CONFIG_REGISTER, retval)

    def useDualPoint(self, b):
        retval = self._read_byte(PH_CONFIG_REGISTER)

        retval = self._bit_set(retval, PH_DUALPOINT_CONFIG_BIT, b)
        self._write_byte(PH_CONFIG_REGISTER, retval)

    def usingTemperatureCompensation(self):
        retval = self._read_byte(PH_CONFIG_REGISTER)
        return (retval >> 1) & 0x01

    def usingDualPoint(self):
        retval = self._read_byte(PH_CONFIG_REGISTER)
        return (retval >> 0) & 0x01

    def _bit_set(self, v, index, x):
        mask = 1 << index
        v &= ~mask
        if x:
            v |= mask
        return v

    def _change_register(self, r):
        global i2c
        i2c.write_byte(self.address, r)
        time.sleep(10 / 1000.0)

    def _send_command(self, command):
        global i2c
        i2c.write_byte_data(self.address, PH_TASK_REGISTER, command)
        time.sleep(10 / 1000.0)

    def _write_register(self, reg, f):
        global i2c
        n = self.round_total_digits(f)
        fd = bytearray(struct.pack("f", n))
        data = [0, 0, 0, 0]
        data[0] = fd[0]
        data[1] = fd[1]
        data[2] = fd[2]
        data[3] = fd[3]
        self._change_register(reg)
        i2c.write_i2c_block_data(self.address, reg, data)
        time.sleep(10 / 1000.0)

    def _read_register(self, reg):
        global i2c
        data = [0, 0, 0, 0]
        self._change_register(reg)
        data[0] = i2c.read_byte(self.address)
        data[1] = i2c.read_byte(self.address)
        data[2] = i2c.read_byte(self.address)
        data[3] = i2c.read_byte(self.address)
        ba = bytearray(data)
        f = struct.unpack('f', ba)[0]
        return self.round_total_digits(f)

    def _write_byte(self, reg, val):
        global i2c
        i2c.write_byte_data(self.address, reg, val)
        time.sleep(10 / 1000.0)

    def _read_byte(self, reg):
        global i2c
        self._change_register(reg)
        time.sleep(10 / 1000.0)
        return i2c.read_byte(self.address)

    def magnitude(self, x):
        if math.isnan(x):
            return 0
        return 0 if x == 0 else int(math.floor(math.log10(abs(x)))) + 1

    def round_total_digits(self, x, digits=7):
        return round(x, digits - self.magnitude(x))
