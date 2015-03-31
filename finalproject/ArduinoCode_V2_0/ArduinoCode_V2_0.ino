
#include <Stepper.h>
#define STEPS 48

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepperPan(STEPS, 8, 9, 10, 11);
Stepper stepperTilt(STEPS, 3,4,5,6);

int panDir;
int tiltDir;
int solenoid = 2;

void setup()
{
  Serial.begin(9600);
  pinMode(solenoid, OUTPUT);
  digitalWrite(solenoid, LOW);
}


void loop() {
  if (Serial.available() > 0){
    // read the Kinect data from python and
    // extrapolate pan/tilt speeds and firing boolean
    int data = Serial.read();
    Serial.println(data);
    panDir = data/10-5;
    tiltDir = data%10-5;
    //Serial.print(panDir);
    //Serial.print(", ");
    //Serial.println(tiltDir);
    
    if (data == 0){
      digitalWrite(solenoid, HIGH);
      Serial.println("FIRE");
      panDir = 0;
      tiltDir = 0;
    }
    else{
      // Step motors based on direction of target
      
    }
  }
  stepperPan.setSpeed(abs(panDir*20));
  stepperTilt.setSpeed(abs(tiltDir*20));
  stepperPan.step(panDir);
  stepperTilt.step(tiltDir);
  delay(10);
  digitalWrite(solenoid, LOW);
}
