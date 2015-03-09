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


### Conclusions
