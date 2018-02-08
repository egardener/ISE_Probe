from iseprobe import iseprobe

ph = phprobe(0x3f, -1, 5, 4)

ph.measurepH()
print("pH: " + str(ph.pH))
print("mV: " + str(ph.mV))
print("pOH: " + str(ph.pOH))
