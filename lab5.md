## Lab 5: Motion Detection
### Introduction
The objective of this lab was to use CUDA and OpenCV to detect vertical hand movements with a webcam. By using CUDA, images in video frames could be processed much faster on the GPU than on the CPU. Unfortunately, I was unable to successfully detect hand movements on the GPU. Instead, I built off of an existing hand detection program to detect two hands and output both their vertical velocities.

Note: All of my modified programs are in the ```/opencv-2.4.9/HandDetection``` directory. 

### Design Methodology
First I would like to review the program that I did create to detect hand movements on the CPU. I started with bengal's opencv-hand-detection repository, and began by analyzing the ```hand.c``` file. This program works as follows.

1. Begin capturing video from webcam
2. Threshold image.

First by applying a gaussian blur and median blur.
Then the video frames are thresholded by HSV ranges to detect skin color.
Finally the image is converted to binary (black/white)

3. Find all contours in the binary image and select the largest.
4. Find the convex hull of the largest contour and look for convexity defects.
5. Find the radius of the hand by computing the distance between defect to the center.
6. Identify fingers as local maximums from the hand center.
7. Identify palm, fingers, and defects by illustrating the image.

The first step was to find the right HSV thresholds so that the program could detect my skin color effectively. I took a screenshot of the webcam feed and looked at HSV values of the pixels of my hand. By specifying a range around the target value for hue, saturation, and value; I was able to effectively threshold my hands. Below are my thresholds.

```c
//Add Thresholds
```

In order to identify not just the hand positions but its movement, I added code to measure the change in vertical hand position inside the ```display()``` function. I chose the ```hand_center ``` as my reference point and measured the change in its position every specified number of frames. If the velocity was measured for every video frame, the data becomes noisy, so I chose to only measure the change in positions every so often (see Testing section for details). 

My next step was not detecting one hand but two hands! My first method involved modifying the ```find_contour()``` function to select both the largest and second largest contours. For both of these contours the convex hull was found and the coordinates of the center of the hull was calculated. Unfortunately, the largest and second largest contours were always in nearly the exact same location. I could not identify two different hands, just two different parts of the same hand. Thus I decided to take a different approach.



### Testing


### Conclusions
