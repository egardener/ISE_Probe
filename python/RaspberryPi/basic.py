from iseprobe import iseprobe

# the passed parameter 3 indicates the third (/dev/i2c-3) I2C device
mv = iseprobe(0x3f, 3)

mv.measuremV()
print("mV: " + str(mv.mV))
