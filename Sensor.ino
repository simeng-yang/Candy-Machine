/*
Authors: Simeng Yang, Ronald Gayoswki, Nathan Sheasby
Item: Skittle Sorting Machine Code
Date: June 16, 2016
*/

#include <SoftwareSerial.h>

const int sio = 2;      // ColorPAL connected to pin 2
const int unused = 255;     // Non-existant pin # for SoftwareSerial
const int sioBaud = 4800;
const int waitDelay = 200;

// Received RGB values from ColorPAL
int red;
int grn;
int blu;

// Set up two software serials on the same pin.
SoftwareSerial serin(sio, unused);
SoftwareSerial serout(unused, sio);

void setup() {
  Serial.begin(9600);
  reset();          // Send reset to ColorPal
  serout.begin(sioBaud);
  pinMode(sio, OUTPUT);
  serout.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
  serout.end();       // Discontinue serial port for transmitting

  serin.begin(sioBaud);         // Set up serial port for receiving
  pinMode(sio, INPUT);
}

void loop() {
readData();
}  

// Reset ColorPAL; see ColorPAL documentation for sequence
void reset() {
  delay(200);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  pinMode(sio, INPUT);
  while (digitalRead(sio) != HIGH);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  delay(80);
  pinMode(sio, INPUT);
  delay(waitDelay);
}

void readData() {
  char buffer[32];
  
  if (serin.available() > 0) {

     // Wait for a $ character, then read three 2 digit hex numbers
    buffer[0] = serin.read();
    if (buffer[0] == '$') {
      for(int i = 0; i < 9; i++) {
        while (serin.available() == 0);     // Wait for next input character
        buffer[i] = serin.read();
        if (buffer[i] == '$')               // Return early if $ character encountered
          return;
      }
      parseAndPrint(buffer);
      delay(50);
    }
 
  }
}

// Parse the hex data into integers
void parseAndPrint(char * data) {
  sscanf (data, "%2x%2x%2x", &red, &grn, &blu);
  char buffer[32];
  sprintf(buffer, "R%4.4d G%4.4d B%4.4d", red, grn, blu);
  Serial.println(buffer);
}
