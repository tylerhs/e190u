
## Final Project: Self-tracking Cannon
### Introduction
For my final project I decided to build from scratch a fully-functional self-tracking cannon. I had the idea from the fall semester in Prof Dodd's robot lab, but I wanted to carry it through using a Jetson TK1 as the onboard computer to track the cannon's target. The main idea was to build a cannon with a wooden frame and steel axles, which uses Pan/Tilt movements using stepper motors. A webcam placed on the cannon's barrel would look for targets and the Jetson would process this data and send data to an Arduino. The Arduino would then control the stepper motors, and an electric solenoid valve. The solenoid valve would release the compressed air in the cannon's tank. 

### Design Methodology
##### Mechanical Design
I first built the cannon itself. After a trip to Lowe's I quickly epoxed the PVC pieces together. I drilled a hole in a "Tee" fitting and inserted a bike tube nozzle so that I could pump air into the tank. I then attached a solenoid valve between the tank and the barrel. After testing I came to the conclusion that the cannon was scary and needed to shoot something other than bouncy balls in order to not draw attention from Campus Security. I shot a bouncy ball straight into the air and it stayed airborne for about 12 seconds before hitting the ground. The physics behind that number indicate that bouncy balls are in fact, not safe. Thus, I transitioned to marshmallows, which was still exciting to shoot, but not overly dangerous. 

Next up was to build the frame. I used a 2'x4' section of particle board, and used the Shopbot to create a perfect circle about 2' in diameter. My intention was to allow the circular piece of particle board to rotate on top of a square base. The cannon itself would be mounted on the circular piece, allowing it to rotate left and right.

![alt text](http://i.imgur.com/Hm1SLOH.jpg "Circular platform rotating atop base")

I then drilled holes for the Panning axle and placed bearings in said holes. The first stepper motor was fixed to the circular base and its power was transmitted to the Panning axle using a 3D printed belt pulley and a timing belt.

![alt text](http://imgur.com/E3ccnQE,HW9SpV4,V82RUda,Tw9KbzV,UsenWsZ,U6oK6Z9,8wvqely,EqSnnfT,Hm1SLOH,ivLSbvP,W23P0Pa#5 "Pan motor and belt pulleys")

I then cut triangular legs and glued them to the base of the cannon for support.

![alt text](http://i.imgur.com/E3ccnQE.jpg "Legs for cannon's base")

I then attached two sections of 2x4" to the circular base to built the Tilting frame. A hole for the axle bearings and a hole for the motor was milled out of the wood, and the axle was fed through the upper hole.

![alt text](http://i.imgur.com/ivLSbvP.jpg "Tilting frame")

Finally, the cannon was strapped to a 2x4" using zipties, and everything was wired up to the Arduino through a pair of Darlington Arrays.

![alt text](http://i.imgur.com/EqSnnfT.jpg "Cannon strapped to 2x4")

![alt text](http://i.imgur.com/UsenWsZ.jpg "Wiring")

##### Software Design
I had a few steps involving software that were implemented to ensure full functionality of the cannon.

The first step was identifying the target. To do this I allowed the user to mouse-click on a pixel, and a C program then found the RGB & HSV values at that pixel. It then thresholded the image by allowing only a small range of similar colors to be accepted. I then used OpenCV to find the largest contour of the thresholded image. The center of this contour I defined as my target.

The next step was communicating with the Arduino. I used Serial communication from C to the Arduino to relay information to the stepper motors. In the C file I used the following command to send data to the Arduino.

```c
fprintf("/dev/ttyACM0", "%c", data);
```

If the target was above the center of the camera's field of view I told the Tilting stepper motor to move the barrel up, and if the target was to the left of the center of the camera's field of view I told the Panning stepper motor to move the barrel to the left. Unfortunately, I was only able to consistently transmit one character of data to the Arduino, so I converted a two-digit integer into its ASCII equivalent and sent that character to the Arduino. I used a square root function to transmit a Pan speed and Tilt speed depending on where the target was on the screen. The square root function meant that the cannon would care more about accuracy the closer it got toward the target (to effectively increase resolution near the target). 

On the Arduino side I used the Stepper.h library to control both the motors, and controlled the solenoid by turning on a transistor. The transistor allowed 18V (through 2 9V batteries) to flow through the solenoid, activating it. 

### Results & Discussion
The cannon works. If given a clear target, it will slowly track that target and the solenoid will discharge the compressed air once the cannon is aiming directly at the target. It shoots marshmallows at relatively high speed, with pretty good precision. Below is a link to the finished cannon.

https://www.youtube.com/watch?v=h2HzqxYksuk

I have a few things that I would like to improve.

First, the camera is directly above the barrel, which means that the barrel itself blocks a portion of the camera's field of view. Sometimes the camera would lose the target when tracking it because the barrel would block the target. If given more time I would also like to put faster DC motors for the Pan/Tilt frame, which would allow me to use PID control theory to improve the speed and smoothness of target tracking. 
