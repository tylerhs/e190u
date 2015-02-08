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
const int LStickH = A0;
const int LStickV = A1;
const int RStickH = A2;
const int RStickV = A3;
const int LStickB = 4;
const int RStickB = 3;
const int SW = 13;
const int KS = 2;

// parameters for reading the joystick:
int range = 18;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range/8;      // resting threshold
int center = range/2;         // resting position value

boolean mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

void setup() {  
  // start read & write from serial 
  Serial.begin(9600);
  Mouse.begin();
  
  // initialize the buttons as inputs:
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(L4, INPUT);
  pinMode(LStickB, INPUT);
  pinMode(RStickB, INPUT);
  pinMode(SW, INPUT);
  pinMode(KS, INPUT);
}

void loop(){
  // read the state of the pushbutton value:
  int R1state = digitalRead(R1);
  int R2state = digitalRead(R2);
  int R3state = digitalRead(R3);
  int R4state = digitalRead(R4);
  int L1state = digitalRead(L1);
  int L2state = digitalRead(L2);
  int L3state = digitalRead(L3);
  int L4state = digitalRead(L4);
  int LStickBstate = 1 - digitalRead(LStickB);
  int RStickBstate = 1 - digitalRead(RStickB);
  int SWstate = digitalRead(SW);
  int KSstate = digitalRead(KS);
  
  int RStickHvalue = 1023 - analogRead(RStickH);
  int RStickVvalue = 1023 - analogRead(RStickV);
  int LStickHvalue = 1023 - analogRead(LStickH);
  int LStickVvalue = 1023 - analogRead(LStickV);
  
  // get joystick readings
  int xReading = -readAxis(RStickH);
  int yReading = readAxis(RStickV);
  if(SWstate == HIGH){
    // move mouse based on joystick movement
    Mouse.move(xReading, yReading,0);
    
    // Mouse clicks
    if(L1state == HIGH){
      Mouse.press(MOUSE_LEFT);
    }
    else{
      Mouse.release(MOUSE_LEFT);
    }
    if(R1state == HIGH){
      Mouse.press(MOUSE_RIGHT);
    }
    else{
      Mouse.release(MOUSE_RIGHT);
    }
    
    // WASD mapping
    if(LStickHvalue > 600){
      Keyboard.press('a');
    }
    else{
      Keyboard.release('a');
    }
    if(LStickHvalue < 420){
      Keyboard.press('d');
    }
    else{
      Keyboard.release('d');
    }
    if(LStickVvalue > 600){
      Keyboard.press('w');
    }
    else{
      Keyboard.release('w');
    }
    if(LStickVvalue < 420){
      Keyboard.press('s');
    }
    else{
      Keyboard.release('s');
    }
    
    // button to key mapping
    if(L2state == HIGH){
      Keyboard.press('q');
    }
    else{
      Keyboard.release('q');
    }
    if(R2state == HIGH){
      Keyboard.press('e');
    }
    else{
      Keyboard.release('e');
    }
    if(KSstate == HIGH){
      Keyboard.press(' ');
    }
    else{
      Keyboard.release(' ');
    }
    if(L3state == HIGH){
      Keyboard.press('1');
    }
    else{
      Keyboard.release('1');
    }
    if(R3state == HIGH){
      Keyboard.press('2');
    }
    else{
      Keyboard.release('2');
    }
    if(L4state == HIGH){
      Keyboard.press('x');
    }
    else{
      Keyboard.release('x');
    }
    if(R4state == HIGH){
      Keyboard.press(0xB1);a
    }
    else{
      Keyboard.release(0xB1);
    }
    delay(responseDelay);
  }
  
  
}

int readAxis(int thisAxis) { 
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  } 

  // return the distance for this axis:
  return distance;
}
```
