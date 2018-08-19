import cmd

from iseprobe import iseprobe

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
ise = iseprobe(0x3f, -1, scl=5, sda=4)


class iseshell(cmd.Cmd):
    prompt = '> '

    def do_config(self, a):
        """prints out all the configuration data\nparameters: none"""
        print("config: ")
        print("\toffset: " + str(ise.getCalibrateOffset()))
        print("\tdual point: " + str(ise.usingDualPoint()))
        print("\tlow reference / read: " + str(ise.getCalibrateLowReference()) + " / " + str(ise.getCalibrateLowReading()))
        print("\thigh reference / reading: " + str(ise.getCalibrateHighReference()) + " / " + str(ise.getCalibrateHighReading()))
        print("\ttemp. compensation: " + str(ise.usingTemperatureCompensation()))
        print("\tversion: " + (hex(ise.getVersion())))

    def do_reset(self, a):
        """reset all saved values\nparameters: none"""
        ise.reset()

    def do_temp(self, temp_C):
        """measures the temperature\nparameters: none"""
        if temp_C:
            ise.setTemp(float(temp_C)

        ise.measureTemp()
        print("C/F: " + str(ise.tempC) + " / " + str(ise.tempF))

    def do_cal(self, solution_pH):
        """calibrates the device\nparameters:\n\tcalibration solution in mS"""
        if solution_pH:
            ise.calibrateSingle(float(solution_pH))

        print("offset: " + str(ise.getCalibrateOffset()))

    def do_mv(self, a):
        """starts a ph measurement\nparameters: none"""
        ise.measuremV()
        print("mV: " + str(ise.mV))

    def do_low(self, low_reference_pH):
        """returns or sets the low reference/reading values\nparameters\n\tlow reference solution in mS"""
        if low_reference_pH:
            ise.calibrateProbeLow(float(low_reference_pH))

        print("\tlow reference / read: " + str(ise.getCalibrateLowReference()) +
              " / " + str(ise.getCalibrateLowReading()))

    def do_high(self, high_reference_pH):
        """returns or sets the high referencen/reading values\nparameters\n\thigh reference solution in mS"""
        if high_reference_pH:
            ise.calibrateProbeHigh(float(high_reference_pH))

        print("\thigh reference / reading: " + str(ise.getCalibrateHighReference()) + " / " + str(ise.getCalibrateHighReading()))

    def do_tc(self, arg):
        """returns or sets temperature compensation information\nparameters\n\tbool to use compensation\n\ttemperature constant to use (255 for actual)"""
        a = arg.split()

        if len(a) >= 1:
            ise.useTemperatureCompensation(int(a[0]))

        print("\ttemp. compensation: " + str(ise.usingTemperatureCompensation()))

    def do_dp(self, useDP):
        """returns or sets dual point use\nparameters\n\tbool 0/1"""
        if useDP:
            ise.useDualPoint(int(useDP))

        print("\tdual point: " + str(ise.usingDualPoint()))

    def do_data(self, a):
        """prints all the data registers"""
        print("mV: " + str(ise.mV))

    def do_version(self, a):
        """prints the version register"""
        print("\tversion: " + (hex(ise.getVersion())))

    def do_i2c(self, i2cAddress):
        """changes the I2C address"""
        ise.setI2CAddress(i2cAddress)

    def do_EOF(self, line):
        return True


iseshell().cmdloop()
