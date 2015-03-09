## Lab 5: Motion Detection
### Introduction
The objective of this lab was to use CUDA and OpenCV to detect vertical hand movements with a webcam. By using CUDA, images in video frames could be processed much faster on the GPU than on the CPU. I decided to build off of an existing hand detection program to detect two hands and output both their vertical velocities. I then ported the image thresholding and filtering to the GPU to increase the FPS. 

Note: All of my modified programs are in the ```/lab5``` directory, I struggled pushing the opencv directory from my local repo to here, but I will try to get that working.

### Dual Hand Detection
First I would like to modifications I made to detect hand movements on the CPU. I started with bengal's opencv-hand-detection repository, and began by analyzing the ```hand.c``` file. This program works as follows.

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
Low HSV: 0, 55, 40
High HSV: 40, 175, 120
```

In order to identify not just the hand positions but its movement, I added code to measure the change in vertical hand position inside the ```display()``` function. I chose the ```hand_center ``` as my reference point and measured the change in its position every specified number of frames. If the velocity was measured for every video frame, the data becomes noisy, so I chose to only measure the change in positions every so often (see Testing section for details). 

My next step was not detecting one hand but two hands! My first method involved modifying the ```find_contour()``` function to select both the largest and second largest contours. For both of these contours the convex hull was found and the coordinates of the center of the hull was calculated. Unfortunately, the largest and second largest contours were always in nearly the exact same location. I could not identify two different hands, just two different parts of the same hand. Thus I decided to take a different approach. I instead used the ```cvRectangle``` function to black out the left and right side of the screen for two different images. I then ran ```cvFindContours()``` on each image, which would find the largest contour on the left and right side of the screen. 
```c
cvRectangle(ctx->ipl_temp_image1, cvPoint(0,0), cvPoint(cvGetSize(ctx->ipl_image).width/2,cvGetSize(ctx->ipl_image).height), BLACK,  CV_FILLED,8, 0 );
```
From these contours I then found the convex hull for the second contour as well (with the same method) and found the center of this convex hull by measuring the distance to each defect point. In the ```display()``` function I then output the vertical velocity of both the left and right hands. I also drew 2 circles on the original image to display the center and size of both hands (in different colors). Below are images of my dual hand detection. The first image shows hand detection without optimal thresholding, and the second image shows hand detection with better thresholding. Note the difference in hand sizes (as calculated by the software).

![alt text](http://i.imgur.com/xpXd8qZ.png "Bad thresholding")

![alt text](http://i.imgur.com/rpMfsSO.png "Good thresholding")



### Testing


### Conclusions
