### pH Probe Interface for Raspberry Pi

Read the [documentation](http://ufire.co/pHProbe/#getting-started) regarding adding an I2C overlay to use software I2C, since the Raspberry's implementation has a bug.

#### Running the Examples
1. git clone https://github.com/u-fire/pHProbe-python.git
2. cd pHProbe/python/RaspberryPi
3. sudo python3 basic.py
4. sudo python3 shell.py

~~~
from phprobe import phprobe

ph = phprobe(3)

print("pH: " + str(ph.measurepH())
~~~
