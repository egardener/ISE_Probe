from iseprobe import iseprobe

POTENTIAL_REGISTER_ADDRESS = 100


class ise_orp(iseprobe):
    ORP = 0
    Eh = 0

    def measureORP(self):
        self.measuremV()
        self.ORP = self.mV
        self.Eh = self.mV + self.getProbePotential()

        return self.mV

    def setProbePotential(self, potential):
        self.writeEEPROM(POTENTIAL_REGISTER_ADDRESS, potential)

    def getProbePotential(self):
        return self.readEEPROM(POTENTIAL_REGISTER_ADDRESS)
