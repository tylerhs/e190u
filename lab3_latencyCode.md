```c
/////////////////////
// Lab 2 Code      //
// Tyler Smallwood //
// E190U           //
/////////////////////

// set pins for buttons, joysticks, and switch
const int R1 = 12;
const int R2 = 11;
const int R3 = 10;
const int R4 = 9;
const int L1 = 8;
const int L2 = 7;
const int L3 = 6;
const int L4 = 5;
const int KS = 2;

const int TRIALS = 15;
int latencies[TRIALS];
int counter = -1;

void setup() {  
  // start read & write from serial 
  Serial.begin(9600);
  // initialize the buttons as inputs:
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(L4, INPUT);
  pinMode(KS, INPUT);
  
  randomSeed(analogRead(4));
}

void loop(){
  // read the state of the pushbutton value:
  if(counter == -1){
    delay(3000);
    Serial.println(" ");
    Serial.println("Press KS to start");
    while(digitalRead(2) == 0){}
    Serial.print("Beginning in 3...");
    delay(1000);
    Serial.print("2...");
    delay(1000);
    Serial.println("1...");
    delay(1000);
    counter++;
  }
  else if(counter < TRIALS){
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
    delay(100);
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
}
  
  ```
