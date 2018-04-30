from iseprobe import iseprobe

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
mv = iseprobe(0x3f, -1, 5, 4)

mv.measuremV()
print("mV: " + str(mv.mV))
