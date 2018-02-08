import cmd

from phprobe import phprobe

ph = phprobe(3)


class pHShell(cmd.Cmd):
    prompt = '> '

    def do_config(self, a):
        """prints out all the configuration data\nparameters: none"""
        print("config: ")
        print("\toffset: " + str(ph.getCalibrateOffset()))
        print("\tdual point: " + str(ph.usingDualPoint()))
        print("\tlow reference / read: " + str(ph.getCalibrateLow()) + " / " + str(ph.getCalibrateLowReading()))
        print("\thigh reference / reading: " + str(ph.getCalibrateHigh()) + " / " + str(ph.getCalibrateHighReading()))
        print("\ttemp. compensation: " + str(ph.usingTemperatureCompensation()))
        print("\tversion: " + (hex(ph.getVersion())))

    def do_reset(self, a):
        """reset all saved values\nparameters: none"""
        ph.reset()

    def do_temp(self, a):
        """measures the temperature\nparameters: none"""
        ph.measureTemp()
        print("C/F: " + str(ph.tempC) + " / " + str(ph.tempF))

    def do_cal(self, solution_pH):
        """calibrates the device\nparameters:\n\tcalibration solution in mS"""
        if solution_pH:
            ph.calibrateSingle(float(solution_pH))

        print("offset: " + str(ph.getCalibrateOffset()))

    def do_ph(self, a):
        """starts a ph measurement\nparameters: none"""
        ph.measurepH()
        print("pH: " + str(ph.pH))

    def do_low(self, low_reference_pH):
        """returns or sets the low reference/reading values\nparameters\n\tlow reference solution in mS"""
        if low_reference_pH:
            ph.calibrateProbeLow(float(low_reference_pH))

        print("\tlow reference / read: " + str(ph.getCalibrateLow()) +
              " / " + str(ph.getCalibrateLowReading()))

    def do_high(self, high_reference_pH):
        """returns or sets the high referencen/reading values\nparameters\n\thigh reference solution in mS"""
        if high_reference_pH:
            ph.calibrateProbeHigh(float(high_reference_pH))

        print("\thigh reference / reading: " + str(ph.getCalibrateHigh()) + " / " + str(ph.getCalibrateHighReading()))

    def do_tc(self, arg):
        """returns or sets temperature compensation information\nparameters\n\tbool to use compensation\n\ttemperature constant to use (255 for actual)"""
        a = arg.split()

        if len(a) >= 1:
            ph.useTemperatureCompensation(int(a[0]))

        print("\ttemp. compensation: " + str(ph.usingTemperatureCompensation()))

    def do_dp(self, useDP):
        """returns or sets dual point use\nparameters\n\tbool 0/1"""
        if useDP:
            ph.useDualPoint(int(useDP))

        print("\tdual point: " + str(ph.usingDualPoint()))

    def do_data(self, a):
        """prints all the data registers"""
        print("pH: " + str(ph.pH))
        print("mV: " + str(ph.mV))
        print("pOH: " + str(ph.pOH))

    def do_version(self, a):
        """prints the version register"""
        print("\tversion: " + (hex(ph.getVersion())))

    def do_i2c(self, i2cAddress):
        """changes the I2C address"""
        ph.setI2CAddress(i2cAddress)

    def do_EOF(self, line):
        return True


if __name__ == '__main__':
    pHShell().cmdloop()
