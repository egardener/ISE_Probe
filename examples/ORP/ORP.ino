/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

 #include <ISE_ORP.h>

ISE_ORP orp;

void setup() {
  Serial.begin(9600);
}

void loop() {
  orp.measureORP();
  Serial.print("mV: "); Serial.println(orp.ORP);
}
