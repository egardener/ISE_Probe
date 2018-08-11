#include "RunningMedian.h"
#include <ISE_pH.h>

ISE_pH pH;
RunningMedian avg_pH;

void setup() {
  Serial.begin(9600);
}

void loop() {
  avg_pH.add(pH.measurepH());
  Serial.println(avg_pH.round0(avg_pH.getAverage()),   1);
  Serial.println(avg_pH.round00(avg_pH.getAverage()),  2);
  Serial.println(avg_pH.round000(avg_pH.getAverage()), 3);
  Serial.println("-----");
}
