from iseprobe import iseprobe


class ise_ph(iseprobe):
    def measurepH(self):
        self.measuremV()
        self.pH = abs(7.0 - ((self.mV / 1000) / PROBE_MV_TO_PH))
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
