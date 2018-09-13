/*!
   ufire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   This is compatible with hardware version 2. 

   This show show to use temperature compensation while taking a pH reading.
   If useTemperatureCompensation is true, a temperature reading is also taken, and an
   adjustment formula is applied. The adjustment is typically slight and not applied within
   certain temperature ranges and pH ranges.
 */

 #include <ISE_pH.h>

ISE_pH ph;

void setup() {
  Serial.begin(9600);
  ph.useTemperatureCompensation(true);
}

void loop() {
  ph.measurepH();

  // to use an outside source of temperature data
  // float your_temp = 20.2; // your temperature routine would supply this value
  // ph.measurepH(your_temp);
  
  Serial.print("pH: "); Serial.println(ph.pH);
  Serial.println("---");
}
