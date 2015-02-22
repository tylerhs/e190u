
## Lab 4: Graphical Post-Processing

### Introduction
The objective of this lab is to use CUDA to as a platform for implementing computationally-intensive image processing techniques. I chose to augment the existing postProcessGL example for this lab. 

### Design Methodology
##### Overview
My project works as follows:

I created a 3x3 grid of solid spheres in the window. I chose gravity to point into the screen (which I will call the positive z direction). The spheres all started at different heights (in the z direction). Once the program began, the spheres fell into the screen by the force of gravity until they hit the floor in which they bounced back up. These collisions were slightly inelastic ( meaning their speed decreased with each bounce). 

![alt text](http://i.imgur.com/7u7ngA8.png "Bouncing Balls")

Now for the image processing part! The height of each ball corresponds to an element in a 3x3 convolution matrix. That is, as the balls are bouncing randomly in the window, a convolution matrix is constantly being calculated based on their heights. This matrix is then processed for every pixel to distort the image. 

##### Implementation
There are two primary tasks in implementing this project. The first involves declaring a convolution matrix and processing the image based on that matrix. The bulk of this task was done in the postProcessGL.cu file. For every dy and dx in the matrix of pixels currently being processed, I multiplied an element in the convolution matrix by the corresponding pixel's r,g,b values. These values were then summed to create the output pixel. 

```c++
// weight pixel rgb values by convolution elements
r *= convMat[index];
g *= convMat[index];
b *= convMat[index];

// sum matrix
rsum += r;
gsum += g;
bsum += b;
```

I struggled with passing an array to the cudaProcess function, so instead I passed 9 floats and defined an array inside the cudaProcess function. I am currently working on passing an array though, as that would result in much cleaner code. 
The modifications to create the bouncing balls were more extensive. First a for loop was implemented to create the solid sphere using `glutSolidSphere()` inside of the `renderScene()` function. The x,y coordinates of each sphere varied based on the counting elements in the for loops. The z coordinate of each sphere started as a pseudo-random value using the following declaration:
```c++
heightMat[xObj][yObj] = (rand()%8)+rand()/2000000000.0;
```
This created a random float between 0 and 9 for each ball. To update the heights with time I made modifications to the `timerEvent` function. Here I created another set of 2 for loops that looped over each ball. Each ball had a corresponding velocity represented in a 2D array, and that velocity was updated based on a constant acceleration. The height was then updated based on the current velocity. If the ball's height reached the floor, the velocity was multiplied by a factor of -0.95 to simulate a loss in kinetic energy. 
```c++
float accel = 0.002;
		for(int xObj = 0; xObj < 3; xObj++){
			for(int yObj = 0; yObj < 3; yObj++){
				if (heightMat[xObj][yObj] > 7){
					
					vel[xObj][yObj] *= -0.95;
					heightMat[xObj][yObj] -= 0.02;
				}
				vel[xObj][yObj] += accel;
				heightMat[xObj][yObj] += vel[xObj][yObj];
			}
		}
```
Now to put everything together. In the `renderScene()` function I defined the convolution matrix to be proportional to the ball height matrix. The tricky part was finding proportionality constants at which the range of convolution values created interested visual effects rather than a white or black screen. 
```c++
convMat[xObj+yObj*3] = (6.5-heightMat[xObj][yObj])/2;
```
That's all he wrote.

### Testing
Below is a video of the project in action.

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/IS_McM2_JFI/0.jpg)](http://www.youtube.com/watch?v=IS_McM2_JFI)

### Results
From the above video, it looks like the image is processed with a convolution based on the height of the balls, but the image isn't distorted very radically. It's possible that the each element in the convolution matrix is based on the average height of all the balls, and not the individual heights. This would explain the resulting simulation, although the code modifications were relatively straightforward and I'm not entirely sure how this could have happened. 

### Conclusions

This lab successfully demonstrated the power of CUDA in performing operations on the GPU. For this project, I multiplied a 3x3 matrix for all 512^2 pixels. Sending these operations to be performed on the GPU results in greater thoroughput because CUDA is designed to perform many operations in parallel. 


