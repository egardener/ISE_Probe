[![GitHub release](https://img.shields.io/github/release/u-fire/pHProbe.svg)]()
[![Codacy grade](https://img.shields.io/codacy/grade/5a3eff9c5b7c49c5ba03cbe2b4435c3e.svg)]()

### pH Probe Interface

> Measure pH easily
* pH
* pOH
* mV
* temperature in Celsius

Available on [Tindie](https://www.tindie.com/products/ufire/ph-probe-interface/)
Make one with the [source code](https://github.com/u-fire/ph-probe-interface) and [schematics](https://upverter.com/ufire/a3c74dc5b3b942a5/pH-Probe-Interface-2018a/).  

#### What it is
An ATTiny85 programmed as an I2C slave, a DS18B20 waterproof temperature probe, and a pH probe. It measures pH in the full range of 0 - 14 and optionally compensates for temperature.

#### Using it
There is extensive [documentation](http://ufire.co/pHProbe/) on the use and setup of the device. This library is in the Particle.io IDE and a python implementation for Raspberry Pi and MicroPython is also available.


~~~
#include <pHProbe.h>
pH_Probe _pH;

pH = _pH.measurepH();
~~~
