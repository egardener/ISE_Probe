from ise_orp import ise_orp

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
orp = ise_orp(sda=19, scl=23)

orp.measureORP()
print("mV: " + str(orp.mV))
print("Eh: " + str(orp.Eh))
