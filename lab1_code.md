```c
/////////////////////
// Lab 1 Code      //
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
  
  Serial.print(LStickHvalue);
  Serial.print("\t");
  Serial.print(LStickVvalue);
  Serial.print("\t");
  Serial.print(RStickHvalue);
  Serial.print("\t");
  Serial.print(RStickVvalue);
  Serial.print("\t");

  if (R1state == HIGH) {     
    Serial.print("R1");
  }
  if (R2state == HIGH) {     
    Serial.print("R2");
  } 
  if (R3state == HIGH) {     
    Serial.print("R3");
  } 
  if (R4state == HIGH) {     
    Serial.print("R4");
  }
  if (L1state == HIGH) {     
    Serial.print("L1");
  }
  if (L2state == HIGH) {     
    Serial.print("L2");
  } 
  if (L3state == HIGH) {     
    Serial.print("L3");
  } 
  if (L4state == HIGH) {     
    Serial.print("L4");
  } 
  if (LStickBstate == HIGH) {     
    Serial.print("L Joystick");
  }
  if (RStickBstate == HIGH) {     
    Serial.print("R Joystick");
  }
  if (SWstate == HIGH) {     
    Serial.print("Switch On");
  }
  if (KSstate == HIGH) {     
    Serial.print("KNEE-SLAPPER");
  }
  Serial.println("");
  delay(50);
}
```
