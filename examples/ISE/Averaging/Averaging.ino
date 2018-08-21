/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This is compatible with hardware version 1a. 

   It shows how to use a running median and simple rounding to take
   more consistent results. 
 */

#include "RunningMedian.h"
#include <uFire_ISE.h>

ISE_Probe mV;
RunningMedian avg_mV;

void setup() {
  Serial.begin(9600);
}

void loop() {
  avg_mV.add(mV.measuremV());
  Serial.println(avg_mV.round0(avg_mV.getAverage()),   1);
  Serial.println(avg_mV.round00(avg_mV.getAverage()),  2);
  Serial.println(avg_mV.round000(avg_mV.getAverage()), 3);
  Serial.println("-----");
}
