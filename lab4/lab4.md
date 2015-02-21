
## Lab 4: Graphical Post-Processing

### Introduction
The objective of this lab is to use CUDA to as a platform for implementing computationally-intensive image processing techniques. I chose to augment the existing postProcessGL example for this lab. 

### Design Methodology
##### Overview
My project works as follows:

I created a 3x3 grid of solid spheres in the window. I chose gravity to point into the screen (which I will call the positive z direction). The spheres all started at different heights (in the z direction). Once the program began, the spheres fell into the screen by the force of gravity until they hit the floor in which they bounced back up. These collisions were slightly inelastic ( meaning their speed decreased with each bounce). 

![alt text](URL "Bouncing Balls")

Now for the image processing part! The height of each ball corresponds to an element in a 3x3 convolution matrix. That is, as the balls are bouncing randomly in the window, a convolution matrix is constantly being calculated based on their heights. This matrix is then calculated for every pixel to distort the image. 

##### Implementation




![alt text](URL "name")

