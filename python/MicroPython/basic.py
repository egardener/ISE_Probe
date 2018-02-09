from iseprobe import iseprobe

mv = iseprobe(0x3f, -1, 5, 4)

mv.measuremV()
print("mV: " + str(mv.mV))
