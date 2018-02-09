from iseprobe import iseprobe

mv = iseprobe(0x3f, 3)

mv.measuremV()
print("mV: " + str(mv.mV))
