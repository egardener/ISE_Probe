from iseprobe import iseprobe

PROBE_MV_TO_PH = 59.2
TEMP_CORRECTION_FACTOR = 0.03


class ise_ph(iseprobe):
    pH = 0
    pOH = 0

    def measurepH(self):
        self.measuremV()
        self.pH = abs(7.0 - (self.mV / PROBE_MV_TO_PH))
        self.pOH = abs(self.pH - 14)

        if self.usingTemperatureCompensation() is True:
            self.measureTemp()

            distance_from_7 = abs(7 - round(self.pH))
            distance_from_25 = floor(abs(25 - round(temp)) / 10)
            temp_multiplier = (distance_from_25 * distance_from_7) * TEMP_CORRECTION_FACTOR

            if (self.pH >= 8.0) and (self.tempC >= 35):
                temp_multiplier *= -1
            if (self.pH <= 6.0) and (temp <= 15):
                temp_multiplier *= -1

            self.pH += temp_multiplier

        return self.pH

    def calibrateSingle(self, solutionpH):
        super(iseprobe, self).calibrateSingle(pHtomV(solutionpH))

    def calibrateProbeHigh(self, solutionpH):
        super(iseprobe, self).calibrateProbeHigh(pHtomV(solutionpH))

    def calibrateProbeLow(self, solutionpH):
        super(iseprobe, self).calibrateProbeLow(pHtomV(solutionpH))

    def pHtomV(self, pH):
        return (7 - pH) * PROBE_MV_TO_PH
