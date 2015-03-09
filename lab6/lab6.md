## Lab 6: Motion Pong Control
### Introduction
The objective of this lab is to use the Hand detection code created in lab 5 to control a paddle in the game Pong. I chose to use the given Pong example and I used sockets to communicate between my C++ program and the Python program to control the pong paddle.

Note: All of my modified code is in the ```/lab6``` directory, I struggled pushing the opencv directory from my local repo to here, but I will try to get that working.

### Design Methodology
I did a fair amount of research on deciding how to relay the information output by my hand detection algorithm to the python program that is running Pong. Instead of using keyboard or mouse control with the C++ hand detection algorithm, I chose to use sockets. A socket creates a connection (usually over a network) between two separate processes over tcp/ip. I used them locally between my C++ program and the Python program. I chose the python program to be the client, and the C++ program to be the server. 

For the client, I initialized the computer name and port, defined a socket and connected to said socket.

```python
import socket
HOST = 'tegra-ubuntu'
PORT = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
```

In the same file I found the Paddle class and added ```data = s.recv(1)``` to the ```update()``` function. This waited for a message from my hand detection algorithm before updating the position of my paddle. Then I modified the following condition statements to move the paddle up depending on the message transmitted by my hand detection code.

```python
if keys[self.upkey] or data == "U":
	if self.rect.top > 20:
		self.rect.top -= self.speed
elif keys[self.downkey] or data == "D":
	if self.rect.bottom < self.area[1]-20:
		self.rect.bottom += self.speed
```

Now the pong game responds both to keyboard inputs and information relayed over the socket! The next step was to send the right information from my hand detection code.

The ```hand_gpu.cpp``` file was my server, so its job was to bind to the socket, listen for connections, and accept the connection over the same port specified in the python code. After accepting the connection it can send and receive data (in this case only send). I first defined the port number ```#define SERVER_PORT htons(50007)``` and then created another initialization function to bind and accept the socket connection.

```c++
void init_comm(struct ctx *ctx)
{
	char buffer[1000];

  int serverSock=socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = SERVER_PORT;
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

  // wait for a client
  /* listen (this socket, request queue length) */
  listen(serverSock,1);
	bzero(buffer, 1000);

  sockaddr_in clientAddr;
  socklen_t sin_size=sizeof(struct sockaddr_in);
  int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);
	ctx->clientNum = clientSock;
}
```

Then in my display function I wrote to the socket connection depending on the velocity of the left hand. I have yet to implement dual hand control, but it is in the works.

```c++
if (y_vel > 10)
	n = write(ctx->clientNum, "D", 1);
if (y_vel < -10)
	n = write(ctx->clientNum, "U", 1);
```

Notice how the program tells the pong paddle to go down if the velocity is positive. This is because the pixel values in the y direction increase as you get further down the image.

### Testing
It works! By running both the hand detection and pong programs simultaneously, I am able to control the paddle by moving my hand in front of the webcam. Unfortunately, it is very difficult to control the pong paddle using the hand's velocity. This is because if you want to move the paddle very far in one direction, it is necessary to keep moving your hand past the webcam's field of view. Thus I instead chose to control pong paddle position using hand position. Here is my modified code:

```c++
if (ctx->hand_center.y > 300)
	n = write(ctx->clientNum, "D", 1);
else if (ctx->hand_center.y < 180)
	n = write(ctx->clientNum, "U", 1);
else
	n = write(ctx->clientNum, "N", 1);
```

If the hand's position is below 180 pixels or above 300 pixels, the program transmits a command to the python game. Otherwise
the program sends a command to keep the paddle still. I found it much easier to control using hand positioning rather than velocity. One note is that for my current setup, the Pong game waits for a command from the hand detection program before updating the position of paddles/ball. Thus if the hand detection algorithm is running slower than the pong game, the pong game slows down. This makes the game easier to play because reaction times don't have to be as fast, however, it may be more interesting/harder to allow the game to keep updating without input from the hand detection program. Below is an image of both programs running simultaneously.

![alt text](http://i.imgur.com/lIcJaBJ.png "Controlling pong with hand detection")


### Conclusions
This lab was successful in using the hand detection algorithms running on the TK1's GPU (and CPU) to control a Pong paddle's up and down movement. If given more time, I would like to have optimized the hand detection program to run faster, possibly by also mapping the ```findContours()``` and ```findConvexHull()``` functions to GPU in addition to the image filtering. This would make the Pong game run at faster frame rates and allow more precise, smoother control. That being said, I was happy to learn how to use things such as sockets to communicate between running processes. 
