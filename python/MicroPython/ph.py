from ise_ph import ise_ph

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
ph = ise_ph(sda=19, scl=23)

ph.measurepH()
print("mV: " + str(ph.mV))
print("pH: " + str(ph.pH))
print("pOH: " + str(ph.pOH))
