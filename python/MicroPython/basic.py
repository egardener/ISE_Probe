from iseprobe import iseprobe

# connect board SCL to pin 5, SDA to pin 4, but any pins can be used. -1 indicates software I2C.
mv = iseprobe(sda=19, scl=23)

mv.measuremV()
print("mV: " + str(mv.mV))
