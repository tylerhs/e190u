
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
}


void loop() {
  if (Serial.available() > 0){
    // read the Kinect data from python and
    // extrapolate pan/tilt speeds and firing boolean
    int data1 = Serial.read();
    int data2 = Serial.read();
    Serial.print(data1);
    Serial.print(", ");
    Serial.println(data2);
    /*if (((data/1000) == 0) and ((data/100) > 0)){
      int panDiff = (data/100)-5;
      int tiltDiff = ((data-panDiff*100)/10)-5-50;
      int fire = data - panDiff*100 - tiltDiff*10-5;
      //Serial.println(panDiff);
      //Serial.println(tiltDiff);
      //Serial.println(fire);
      
      // Set the speed of each motor 
      stepperPan.setSpeed(abs(panDiff)*5);
      //stepperTilt.setSpeed(abs(tiltDiff)/10);
      
      // Bombs away!
      if (fire == 1){
        //Engage Solenoid
        Serial.println("FIRE");
      }
      else{
        // Step motors based on direction of target
        if (panDiff > 0){
          Serial.println("Pan Left");
          stepperPan.step(100);
        }
        else{
          Serial.println("Pan Right");
          stepperPan.step(-1);
        }
        
        
        if (tiltDiff > 0){
          stepperTilt.step(1);
        }
        else{
          stepperTilt.step(-1);
        }
      }
    }*/
  }
}
