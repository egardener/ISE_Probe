from ise_orp import ise_orp

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
orp = ise_orp(0x3f, -1, 5, 4)

orp.measureORP()
print("mV: " + str(orp.mV))
print("Eh: " + str(orp.Eh))
