
## Lab 1: Build a Gamepad Controller

### Introduction
In this lab I created a fully functional video game controller with an Arduino Micro as the microcontroller. I built the base design from scratch using a section of 2 by 4, and configured the button and joystick positions in a unique manner to be both intuitive to use and comfortable to hold. The buttons were soldered with wire to a pair of breadboards, while the joysticks were wired directly using Male to Female Headers. Finally, I included a surprise "knee-slapper" button on the bottom of the controller to be used as a special button (such as a missile launcher or turbo boost) in gameplay.

### Controller Build
I chose to ditch the provided plywood controller entirely when I approached this assignment. Instead I built my controller from scratch using scrap two by four in the machine shop.

![alt text](http://i.imgur.com/euExAvc.jpg "Full Controller")

I went through the following procedure to build my controller:

1. Cut wood 6" long using a hacksaw.
2. Mill slots for joysticks using 1/4"  and 1/2" endmills.

   ![alt text](http://i.imgur.com/K8gVV7t.jpg "Milled slots")

3. Round edges using a vertical grinder.
4. Sand controller by hand for smooth finish.

### Design Methodology
A few factors went into my controller design. First, I wanted the form to be unique. Now that all of the major game consoles are using almost the exact same controller layout, it was important to me to build something different that still felt comfortable. My only real constraint was that the joysticks must be operated by my thumbs because fingers don't have the same dexterity needed to operate the main game controls.

![alt text](http://i.imgur.com/8sg2NUQ.jpg "Joysticks")


I came up with this layout of the hands because it seemed the most natural regarding hand positioning. In addition having all 8 fingers laying against the backside of the controller meant it would decrease the latency of pushing buttons compared to the standard video game controller.

![alt text](http://i.imgur.com/rNsAUDN.jpg "Backside")

The joysticks were inserted into the controller rather than on top in order to decrease their height for increased comfort. I believe this height is fairly optimal, although in hindsight I think the joysticks would be more comfortable to operate if their were tilted slightly toward the user. 

The choices made in wiring were mostly made to keep wires away from the user's hands. The button wires were fed up over the center of the controller away from the thumbs, and the joystick wires were fed down next to the breadboards before being hooked up to the Arduino. The 8 buttons were all hooked up using pull-down 10K ohm resistors. The joysticks were fed 5V and GND; their potentiometers were wired directly to analog inputs A0-A3, and their buttons were wired directly to digital pins. The mode switch at the bottom of the controller was fed to a digital pin and GND using an internal pullup implemented in software. 

![alt text](http://i.imgur.com/Fk36P9M.jpg "Wiring")

##### The Knee-Slapper
The knee-slapper is entirely my own creation (I think). It consists of a ninth button hidden under a spring-loaded bumper. The thought behind this ridiculous looking idea is to provide a more interesting action to operate special controls in video gameplay. For example, in a FPS game this could be "throw a grenade", or in an auto racing game the knee-slapper could activate nitrous. Pressing a simple button on a standard controller doesn't correspond well to the intensity of throwing a hand grenade, but slamming the controller down on your leg is a bit more indicative of the action the user is trying to make. 

![alt text](http://i.imgur.com/f0hU332.jpg "Knee-Slapper")

### Testing Methodology
I chose to test my controller entirely in the Arduino software. I setup my buttons as digital inputs and my joysticks as analog inputs. Using the following functions I printed the state of each sensor to the serial window to test that everything was working:

```c
pinMode(R1, INPUT);
int R1state = digitalRead(R1);
if (R1state == HIGH) {     
   Serial.print("R1");
}
```

```c
int RStickXvalue = analogRead(A0);
Serial.print(RStickXvalue);
```

The actual code is in a separate file, the above code is just a snapshot of how I tested my controller. Here's a video of the Serial monitor during testing:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=DcaYZh32Ow4
" target="_blank"><img src="http://img.youtube.com/vi/DcaYZh32Ow4/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

### Results & Discussion
I felt this lab was very successful. By utilizing tools in the machine shop, I created a very unique video game controller with full functionality for modern games. The design is very ergonomic and natural to hold, and provides easy access to 8 buttons. The addition of the Knee-Slapper provides a refreshing twist on ordinary gameplay, allowing users to make special moves in the game with a more intensive input. 

My design does have a few drawbacks, however. The design feels natural to hold, but the controller isn't as secure in the hands (in my experience at least). The joysticks also don't feel quite centered on my thumbs. If I had more time I would increase the thickness of the controller so that the hands touched more surface area. In addition I would tilt the joysticks towards the user so they could be controlled more easily.

### Conclusions
For this assignment I built from scratch a video game controller using buttons, joysticks, and a switch. I connected these input devices to an Arduino Micro, and tested their functionality in the Arduino software. The design I chose is unique and feels very natural to hold, with easy access to all 8 buttons. It took about two hours to build the wood frame, two hours to attach and wire the sensors, and one hour to test the controller in software. I felt the assignment was very well explained and I definitely enjoyed working on the lab.
