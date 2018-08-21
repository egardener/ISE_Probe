from iseprobe import iseprobe
import math

PROBE_MV_TO_PH = 59.2
TEMP_CORRECTION_FACTOR = 0.03


class ise_ph(iseprobe):
    pH = 0
    pOH = 0

    def measurepH(self, temp_C=None):
        self.measuremV()
        if self.mV == -1:
            self.pH = -1
            self.pOH = -1
            return -1

        self.pH = abs(7.0 - (self.mV / PROBE_MV_TO_PH))
        self.pOH = abs(self.pH - 14)

        if self.usingTemperatureCompensation() is True:
            if temp_C is not None:
                self.measureTemp()
            else:
                self.tempC = temp_C

            distance_from_7 = abs(7 - round(self.pH))
            distance_from_25 = floor(abs(25 - round(temp)) / 10)
            temp_multiplier = (distance_from_25 * distance_from_7) * TEMP_CORRECTION_FACTOR

            if (self.pH >= 8.0) and (self.tempC >= 35):
                temp_multiplier *= -1
            if (self.pH <= 6.0) and (temp <= 15):
                temp_multiplier *= -1

            self.pH += temp_multiplier

        if self.pH <= 0.0 or self.pH >= 14.0:
            self.pH = -1
            self.pOH = -1
        if math.isnan(self.pH):
            self.pH = -1
            self.pOH = -1
        if math.isinf(self.mV):
            self.pH = -1
            self.pOH = -1
        return self.pH

    def calibrateSingle(self, solutionpH):
        super(iseprobe, self).calibrateSingle(pHtomV(solutionpH))

    def calibrateProbeHigh(self, solutionpH):
        super(iseprobe, self).calibrateProbeHigh(pHtomV(solutionpH))

    def getCalibrateHighReference(self):
        return self.mVtopH(super(iseprobe, self).getCalibrateHighReference())

    def getCalibrateHighReading(self):
        return self.mVtopH(super(iseprobe, self).getCalibrateHighReading())

    def calibrateProbeLow(self, solutionpH):
        super(iseprobe, self).calibrateProbeLow(pHtomV(solutionpH))

    def getCalibrateLowReference(self):
        return self.mVtopH(super(iseprobe, self).getCalibrateLowReading())

    def getCalibrateLowReading(self):
        return self.mVtopH(super(iseprobe, self).getCalibrateLowReading())

    def pHtomV(self, pH):
        return (7 - pH) * PROBE_MV_TO_PH

    def mVtopH(self, mV):
        return abs(7.0 - (mV / PROBE_MV_TO_PH));
