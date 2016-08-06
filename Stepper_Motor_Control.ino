/*
Authors: Simeng Yang, Ronald Gayowski, Nathan Sheasby
Item: Skittle Sorting Machine Code
Date: June 16, 2016
*/

//  Interrupt driven Servo library for Arduino
#include <Servo.h>

//  STEPPER MOTOR
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

int steps = 0;

boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
long time;

//  SERVO MOTOR
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup()
{
//  STEPPER
Serial.begin(9600); 
//  `Set to stepper pins to outputs
pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 
}

//  Rotates servo through rotation between positions specified by start and end parameters
//  Change is the increment (or decrement); change > 1 results in choppy motion 
void runServo(int start, int end, int change) {
  myservo.attach(12);  // attaches the servo on pin 12 to the servo object
  
  for (pos = start; pos <= end; pos += change) {
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position 'end' from 'start'
      delay(10);                       // waits 10ms for the servo to reach the position     
  }
  
  delay(5);
}

//  Rotates stepper required # of steps to empty a sector
//  In our implementation, there were 6 sectors in the wheel, with each being 60 degrees
//  Full 360 is 4096 steps. Required steps is 4096 / 6 or about 700, with some upperbound leeway.
void runStepper(int steps){
  while(steps>0){
  currentMillis = micros();
  if(currentMillis-last_time>=1000){
  stepper(1); 
  time=time+micros()-last_time;
  last_time=micros();
  steps--;
  }
  }
  delay(2000);
  //  If you want to change direction, refer to ChangeDirection below
  Direction=!Direction;
}

//  Runs through servo + stepper rotation
//  Note that the effective range of rotation (total) is 120,
//  as constrained by set-up dimensions,
//  although the servo has 180 rotation capability 
void loop()
{
  //  There are 4 cups, so i < 5
  for (int i = 0; i < 5; i++){
      //  Parameters are: start position, end position and increment size
      runServo(30 + 24 * (i-1), 30 + 24 * i, 1);
  }
  
  //  Parmameter is: # of steps (or angle of rotation)
  //  Goes through one sector
  runStepper(700);
}

//  Different output sequences to rotate the stepper motor
void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
}
//  Sets direction (although we did not have to change direction here)
SetDirection();
}
} 

//  Function to change direction of stepper motor
void SetDirection(){
//  If you want to change direction upon function call, change ++ to --
//  for one of the conditions
if(Direction==1){ steps++;}
if(Direction==0){ steps++; }
if(steps>7){steps=0;}
if(steps<0){steps=7; }
}
