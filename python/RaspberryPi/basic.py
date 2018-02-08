from phprobe import phprobe

ph = phprobe(3)

ph.measurepH()
print("pH: " + str(ph.pH))
print("mV: " + str(ph.mV))
print("pOH: " + str(ph.pOH))
