/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone
 */

#include <pHProbe.h>

pH_Probe pH;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  pH.measurepH();
  Serial.print("pH: "); Serial.println(pH.pH);
  Serial.print("pOH: "); Serial.println(pH.pOH);
  Serial.print("mV: "); Serial.println(pH.mV);
  Serial.println("-----");
  delay(10000);
}
