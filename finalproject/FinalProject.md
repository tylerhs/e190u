
## Final Project: Self-tracking Cannon
### Introduction
For my final project I decided to build from scratch a fully-functional self-tracking cannon. I had the idea from the fall semester in Prof Dodd's robot lab, but I wanted to carry it through using a Jetson TK1 as the onboard computer to track the cannon's target. The main idea was to build a cannon with a wooden frame and steel axles, which uses Pan/Tilt movements using stepper motors. A webcam placed on the cannon's barrel would look for targets and the Jetson would process this data and send data to an Arduino. The Arduino would then control the stepper motors, and an electric solenoid valve. The solenoid valve would release the compressed air in the cannon's tank. 

### Design Methodology
##### Mechanical Design
I first built the cannon itself. After a trip to Lowe's I quickly epoxed the PVC pieces together. I drilled a hole in a "Tee" fitting and inserted a bike tube nozzle so that I could pump air into the tank. I then attached a solenoid valve between the tank and the barrel. After testing I came to the conclusion that the cannon was scary and needed to shoot something other than bouncy balls in order to not draw attention from Campus Security. I shot a bouncy ball straight into the air and it stayed airborne for about 12 seconds before hitting the ground. The physics behind that number indicate that bouncy balls are in fact, not safe. Thus, I transitioned to marshmallows, which was still exciting to shoot, but not overly dangerous. 

Next up was to build the frame. I used a 2'x4' section of particle board, and used the Shopbot to create a perfect circle about 2' in diameter. My intention was to allow the circular piece of particle board to rotate on top of a square base. The cannon itself would be mounted on the circular piece, allowing it to rotate left and right.

![alt text](https://lh6.googleusercontent.com/2m3x_1xg1MmncqUqA8PJ_NUkWygsCfupK2JP7SFXmLf-X96CvNl1RnoIfj32Bmx-61qc_6-R8bwbrNE=s800 "Circular platform rotating atop base")

I then drilled holes for the Panning axle and placed bearings in said holes. 

![alt text](https://lh3.googleusercontent.com/qSxdIBXxNMH6OXTym1XREDI4ElJ5i7VxBnXeNJZLLfCNtusxy21IycuS4SkQPxvLABulvFYXwCb6mNw=w918-h797 "Pan motor and belt pulleys")


![alt text](https://lh4.googleusercontent.com/IIZkQ5Oj1ApKvgxjgKDUvXR3wIuN3l2vcsruVQNytVm9kEfMQpkV6logfJLrjwXKC2yOHEQ3Kb-dNsM=w918-h797 "Legs for cannon's base")

![alt text](https://lh5.googleusercontent.com/QsYyzwivggqK0_oh1Pq0_BZ2p3yzLC_o-31fHeNkr2VKi5dbyhtn3e56w-u0dKUjPoOxF1vPamSE__U=s800 "Cannon strapped to 2x4")

##### Software Design

### Results & Discussion

### Conclusions


```c

```

![alt text](http://i.imgur.com/aK5NewV.png "Excel Spreadsheet")




