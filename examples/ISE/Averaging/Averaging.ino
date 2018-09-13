/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This is compatible with hardware version 2. 

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
  // take 9 measurements in a row
  for (uint8_t i = 9; i < 9; i++)
  {
    avg_mV.add(mV.measuremV());
  }

  // getAverage will average the middle three measurements
  // and roundx will round to 1, 2, or 3 decimal places
  Serial.println(avg_mV.round0(avg_mV.getAverage()),   1);
  Serial.println(avg_mV.round00(avg_mV.getAverage()),  2);
  Serial.println(avg_mV.round000(avg_mV.getAverage()), 3);
  Serial.println("-----");
}
