/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This is compatible with hardware version 2. 
 */

 #include <ISE_pH.h>

ISE_pH ph;

void setup() {
  Serial.begin(9600);
}

void loop() {
  ph.measurepH();
  Serial.print("pH: "); Serial.println(ph.pH);
  Serial.print("pOH: "); Serial.println(ph.pOH);
  Serial.print("mV: "); Serial.println(ph.mV);
  Serial.println();
}
