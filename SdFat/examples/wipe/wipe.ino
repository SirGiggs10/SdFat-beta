// Example to wipe all data from an already formatted SD.
#include <SPI.h>
#include "SdFat.h"
const int chipSelect = SS;

SdFat sd;

void setup() {
  int c;
  Serial.begin(9600);
  while (!Serial) {}  // wait for Leonardo
  Serial.println("Type 'Y' to wipe all data.");
  while ((c = Serial.read()) <= 0) {}
  if (c != 'Y') {
    sd.errorHalt("Quitting, you did not type 'Y'.");
  }
  if (!sd.begin(chipSelect)) {
    sd.initErrorHalt();
  }
  // Use wipe() for no dot progress indicator.
  if (!sd.wipe(&Serial)) {
    sd.errorHalt("Wipe failed.");
  }
  // Must reinitialize after wipe.
  if (!sd.begin(chipSelect)) {
    sd.errorHalt("Second init failed.");
  }
  Serial.println("Done");
}

void loop() {
}
