
## Lab 3: Gameplay Testing

### Introduction
The objective of this lab is to quantitatively analyze the performance of my controller. I will do so using two approaches. The first is to use the Arduino IDE to find the reaction times of fingers pressing buttons. The second is to modify the Cube 2 source code to output valuable game statistics. For both methods I will compare my controller to Prof Spjut's base controller design (whose button/joystick layouts are radically different). 

### Software Analysis
##### Button Latency
In gameplay it is important for the user to react quickly to situations on the screen by performing various actions. Many of these actions require button presses, so to analyze one aspect of my controller's performance I decided to calculate the average reaction times to press the primary buttons. To do this I decided to use the Arduino IDE for its ease of use.

The primary implementation in my code is using two arrays, one with the pin #s of the buttons, and the other with the names of the buttons. I used the Arduino `randomSeed();` and `random()` functions to choose a button at random and print its name to the screen. The Arduino then counts (in milliseconds) until the user presses the correct button. Then the process repeats for a user-defined number of trials; at the end the Arduino prints the average reaction time across the whole test. The entire sketch is included in this git, but here are the primary code blocks:

```c
if(counter < TRIALS){
    int buttons[] = {8,7,12,11,2};
    String buttonNames[] = {"L1", "L2", "R1", "R2","KS"};
    int randNum = int(random(5));
    Serial.print("Press ");
    Serial.println(buttonNames[randNum]);
    unsigned long start = millis();
    while(digitalRead(buttons[randNum]) == 0){
    }
    latencies[counter] = millis() - start;
    //Serial.println(millis() - start);
    counter++;
  }
  
else if(counter == TRIALS){
    float sum = 0;
    for(int i=0;i<TRIALS;i++){
      sum += latencies[i];
    }
    float average = (sum/TRIALS)/1000;
    Serial.print("You're average time is: ");
    Serial.println(average);
    counter = -1;
  }
```

I also created a similar sketch for Prof Spjut's controller, where the only modifications were the button pin #s and the button names. 

Note: For my controller I only chose the top four buttons on the backside, because these are the only buttons I use during gameplay. I also included the "Knee-Slapper" which is also a primary button (used for jump). 

##### Game Statistics
The second method at testing performance was in-game statistics. This part was very challenging for me to implement because I've had very little experience in analyzing large amounts of code, (and none in C++). Nevertheless, after hours of debugging, I was able to successfully make modifications to 2 files in the Cube 2 source code.

```c++
server.cpp
```
The server file is the main driver behind useful game statistics. This file performs tasks such as saving the game, inflicting damage on players, defining player info, and much more. My approach was to solely look at "frags" or kills of every player. In the `dodamage()` function, there is a condition for `if(ts.health<=0)`. This is the block that is run if a player is killed. I added code to output to a text file the "client number" of the person who made the kill. 

```c
if(ts.health<=0)
{
  FILE * pFile;
  pFile = fopen("data.txt", "ab");
  fprintf(pFile, "%d, %s \n", actor->clientnum, "Kill");
  fclose(pFile);


```
In this case "data.txt" is the output file to be written to, "ab" is the mode at which the file is written (append mode). The "actor" is the person inflicting damage, and every player has his own "clientnum". By outputting every time a player kills another player, I can easily document both the kills and deaths of the whole game (excluding suicides). This should be a valuable statistic as it encompasses every part of the controller. It does introduce a lot of biases which I will attempt to minimize in the Testing section.

```
weapon.cpp
```
The second file I chose to analyze was the weapon file, which handles weapon cycling, shooting, ammo, and much more. I wanted to gather statistics on player accuracy, so I modified the `shoot()` function. 

```c
if(d->gunselect) {
  d->ammo[d->gunselect]--;

  FILE * pFile;
  pFile = fopen("data.txt", "ab"); 
  fprintf(pFile, "%d, %s \n",d->clientnum, "Shot");
  fclose(pFile);
}
```

After the ammo is subtracted from the player's gun, I placed code to write to the same "data.txt" file which player made a shot. 

From this point, all I have is a text file of Kills and Shots which looks like this:
```
128, Shot
128, Shot
0, Shot
128, Shot
128, Kill
0, Shot
0, Shot
128, Shot
128, Shot
0, Shot
128, Shot
0, Kill
```

I then imported this file into Excel as a CSV file, and used four IF statements to create columns of 1s and 0s depending on which combination of player number and kill/shot each row was. Then I summed the Kills columns to find how many kills each player made, and divided total shots over total kills to give an accuracy statistic for each player. Here is a picture of the Excel spreadsheet:

![alt text](http://i.imgur.com/aK5NewV.png "Excel Spreadsheet")

Using both the reaction times and the gameplay statistics should give me a pretty good idea of how my controller compares to a more classical controller layout.

### Testing and Results
##### Gameplay Testing
There were a few factors that went into gameplay testing. In general, I wanted a very controlled, consistent environment so that the amount of uncontrolled variables were reduced. I chose to implement a 1v1 game vs. a computer for all of my testing. The Free-For-All game mode was chosen for its simplicity, and the Fanatic Quake map was chosen because it is relatively small so there is more action during the game. To reduce skill bias, I used the ABBA method so that the second controller used was not given an advantage due to the player learning how to play. Finally, I found another person with almost no gaming experience to test both the controllers. Here are the results:

| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |

### Conclusions
