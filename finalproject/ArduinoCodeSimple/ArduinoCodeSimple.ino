
#include <Stepper.h>
#define STEPS 48

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepperPan(STEPS, 8, 9, 10, 11);
Stepper stepperTilt(STEPS, 4, 5, 6, 7);

// the previous reading from the analog input

void setup()
{
  Serial.begin(9600);
  // set the speed of the motor to 30 RPMs
  // Set the speed of each motor 
  stepperPan.setSpeed(120);
  stepperTilt.setSpeed(120);
}


void loop() {
  if (Serial.available() > 0){
    // read the Kinect data from python and
    // extrapolate pan/tilt speeds and firing boolean
    int data = Serial.read() - 48;
    //Serial.print(data);
    int panDir = data%3 - 1;
    int tiltDir = (data-1)/3 - 1;
    Serial.print(panDir);
    Serial.print(", ");
    Serial.println(tiltDir);
    
      
   
    
    if (data == 0){
      //Engage Solenoid
      Serial.println("FIRE");
    }
    else{
      // Step motors based on direction of target
      stepperPan.step(panDir);
      stepperTilt.step(tiltDir);
    }
  }
}
