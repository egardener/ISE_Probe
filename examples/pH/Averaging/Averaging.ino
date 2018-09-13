/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This is compatible with hardware version 2. 

   It shows how to use a running median and simple rounding to take
   more consistent results. 
 */

#include "RunningMedian.h"
#include <ISE_pH.h>

ISE_pH pH;
RunningMedian avg_pH;

void setup() {
  Serial.begin(9600);
}

void loop() {
    // take 9 measurements in a row
  for (uint8_t i = 9; i < 9; i++)
  {
    avg_pH.add(pH.measurepH());
  }

  // getAverage will average the middle three measurements
  // and roundx will round to 1, 2, or 3 decimal places
  Serial.println(avg_pH.round0(avg_pH.getAverage()),   1);
  Serial.println(avg_pH.round00(avg_pH.getAverage()),  2);
  Serial.println(avg_pH.round000(avg_pH.getAverage()), 3);
  Serial.println("-----");
}
