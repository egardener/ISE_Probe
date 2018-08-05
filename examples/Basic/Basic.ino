/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

 #include <uFire_ISE.h>

ISE_Probe mv;

// multiple probes can be connected using different I2C addresses. Install the
// shell example and change the address with 'i2c 65' for example, then create a
// new instance using:
// ISE_Probe mv2(65);

void setup() {
  Serial.begin(9600);
}

void loop() {
  mv.measuremV();
  Serial.print("mV: "); Serial.println(mv.mV);
}
