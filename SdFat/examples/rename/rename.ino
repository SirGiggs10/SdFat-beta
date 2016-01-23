/*
 * This program demonstrates use of SdFile::rename()
 * and SdFat::rename().
 */
#include <SPI.h>
#include "SdFat.h"

// SD chip select pin
const uint8_t chipSelect = SS;

// file system
SdFat sd;

// Serial print stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt(F(s))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Leonardo

  cout << F("Insert an empty SD.  Type any character to start.") << endl;
  while (Serial.read() <= 0) {}
  delay(400);  // catch Due reset problem

  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  // Remove file/dirs from previous run.
  if (sd.exists("dir2/DIR3/NAME3.txt")) {
    cout << F("Removing /dir2/DIR3/NAME3.txt") << endl;
    if (!sd.remove("dir2/DIR3/NAME3.txt") ||
        !sd.rmdir("dir2/DIR3/") ||
        !sd.rmdir("dir2/")) {
      error("remove/rmdir failed");
    }
  }
  // create a file and write one line to the file
  SdFile file("Name1.txt", O_WRITE | O_CREAT);
  if (!file.isOpen()) {
    error("Name1.txt");
  }
  file.println("A test line for Name1.txt");

  // rename the file name2.txt and add a line.
  // sd.vwd() is the volume working directory, root.
  if (!file.rename(sd.vwd(), "name2.txt")) {
    error("name2.txt");
  }
  file.println("A test line for name2.txt");

  // list files
  cout << F("------") << endl;
  sd.ls(LS_R);

  // make a new directory - "Dir1"
  if (!sd.mkdir("Dir1")) {
    error("Dir1");
  }

  // move file into Dir1, rename it NAME3.txt and add a line
  if (!file.rename(sd.vwd(), "Dir1/NAME3.txt")) {
    error("NAME3.txt");
  }
  file.println("A line for Dir1/NAME3.txt");

  // list files
  cout << F("------") << endl;
  sd.ls(LS_R);

  // make directory "dir2"
  if (!sd.mkdir("dir2")) {
    error("dir2");
  }

  // close file before rename(oldPath, newPath)
  file.close();

  // move Dir1 into dir2 and rename it DIR3
  if (!sd.rename("Dir1", "dir2/DIR3")) {
    error("dir2/DIR3");
  }

  // open file for append in new location and add a line
  if (!file.open("dir2/DIR3/NAME3.txt", O_WRITE | O_APPEND)) {
    error("dir2/DIR3/NAME3.txt");
  }
  file.println("A line for dir2/DIR3/NAME3.txt");
  file.close();

  // list files
  cout << F("------") << endl;
  sd.ls(LS_R);

  cout << F("Done") << endl;
}
void loop() {}
