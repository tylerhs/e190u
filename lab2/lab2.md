![alt text](URL "NAME")
## Lab 2: Gamepad Firmware

### Introduction
The objective of this lab is to use Arduino's IDE to map the video game controller's inputs to mouse and keyboard inputs. In the previous lab, all of the physical inputs were tested using Serial.print, but in this lab Arduino's Mouse and Keyboard libraries were used to actual control a computer's inputs with the controller.

### Design Methodology
Arduino has a built-in library that allows a compatible Arduino to act as a native mouse or keyboard when connected to a computer. In order to control the mouse and keyboard, I placed conditional statements that moved the cursor or pressed a key if a joystick was moved or a button was pressed. I used the following functions to establish control:

```c
Mouse.move(xPos,yPos,0);
Mouse.press(MOUSE_LEFT);
Keyboard.press('key');
Keyboard.release('key');
```

The 'Keyboard.press()' and 'Keyboard.release()' functions were used because then the user could hold down buttons and they keys would be held down accordingly. The 'Keyboard.write()' function just presses a key for a moment and releases, which is not ideal in gameplay. The following mappings were used from controller to PC:

- Right Joystick > Mouse
- Left Joystick > WASD keys
- L1 button > Mouse Left Button
- R1 button > Mouse Right Button
- L2 button > Q key
- R2 button > E key
- L3 button > 1 key
- R3 button > 2 key
- L4 button > X key
- R4 button > ESC key
- Switch > Toggle Mouse/Keyboard control
- Knee-slapper > Space key

It seemed obvious that one of the joysticks should be mapped to the mouse, and in most games the WASD controls are used for player or camera movement, so the other joystick was mapped to these keys. As for the buttons, the keys they were mapped to were somewhat random, which isn't a problem because game controls can be customized. 

### Testing Methodology
The testing procedure was fairly straightforward, although debugging was made much more difficult when the Arduino took complete control over the mouse and keyboard. It was essential that the controller's switch was used to completely deactivate mouse/keyboard control. I had the most issues with the keyboard functions, as I originally used `Keyboard.write()` which caused some problems. I also tweaked the mouse constants so I had faster and more refined control over the mouse. Here are my tweaked values:

```c
int range = 18;
int responseDelay = 5;
int threshold = range/8;
int center = range/2;
```

### Conclusions
This lab was successful in that I created software that mapped controller inputs to mouse and keyboard control. The joysticks and buttons were mapped in a convenient manner such that they could be used to play CUBE 2 and Open Arena in the next lab. 

