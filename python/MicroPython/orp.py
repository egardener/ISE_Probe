from ise_orp import ise_orp

orp = ise_orp(0x3f, -1, 5, 4)

orp.measureORP()
print("mV: " + str(orp.mV))
print("Eh: " + str(orp.Eh))
