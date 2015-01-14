#include <Servo.h> 
#include <EEPROM.h>

int address = 0; 
Servo servo; 

const int START_POT = 2;
const int LIGHT_POT = 4;
const int HEAT_POT = 7;
const int BUTTON_POT = 8;
const int SERVO_POT = 9;

int startVal;
int lightVal;
int heatVal;
int buttonVal;
int heading;
int goTo;

enum Selector{
  START, LIGHTS, HEAT
};

enum Button{
  GO, HOLD
};

enum Location{
  OFF = 0, ON = 120, LIGHT = 45, HEATS = 90, CHANGING = -1
};

Selector action;
Button opperation;

void checkPositions(){
  startVal = digitalRead(START_POT);
  lightVal = digitalRead(LIGHT_POT);
  heatVal = digitalRead(HEAT_POT);
  if(startVal == 1){
    action = START;
  }else if(lightVal == 1){
    action = LIGHTS;
  }else if(heatVal == 1){
    action = HEAT;
  }
}

void checkButton(){
  buttonVal = digitalRead(BUTTON_POT);
  delay(1);
  if(buttonVal == 1){
    opperation = GO;
  }else{
    opperation = HOLD;
  }
}

void startLoop(){
  delay(10);
  goTo += 10;
  runMotor(CHANGING, goTo);
  buttonVal = digitalRead(BUTTON_POT); 
  if(buttonVal == 1){
    return startLoop();
  }else if(buttonVal == 0 && goTo > 130){
    runMotor(CHANGING, (goTo - 30));
    goTo = 0;
  }
}

void runMotor(int place, int _heading){
  if(_heading == ON){
    startLoop();
  }
  servo.write(_heading);
  heading = _heading;
  EEPROM.write(address, heading);
  if(place != CHANGING){
    delay(1200);
  }
}

void exicute(){
  if(heading == OFF && action == START && opperation == GO){
    runMotor(heading, ON);
  }else if(heading == ON && action == START && opperation == GO){
    runMotor(heading, OFF);
  }else if(heading == OFF && action == LIGHTS && opperation == GO){
    runMotor(heading, LIGHT);
  }else if(heading == ON && action == LIGHTS && opperation == GO){
    runMotor(heading, LIGHT);
  }else if(heading == ON && action == LIGHTS && opperation == GO){
    runMotor(heading, LIGHT);
  }else if(heading == LIGHT && action == LIGHTS && opperation == GO){
    runMotor(heading, OFF);
  }else if(heading == LIGHT && action == START && opperation == GO){
    runMotor(heading, ON);
  }else if(heading == LIGHT && action == HEAT && opperation == GO){
    runMotor(heading, HEATS);
  }else if(heading == OFF && action == HEAT && opperation == GO){
    runMotor(heading, HEATS);
  }else if(heading == HEATS && action == HEAT && opperation == GO){
    runMotor(heading, OFF);
  }else if(heading == HEATS && action == START && opperation == GO){
    runMotor(heading, ON);
  }else if(heading == ON && action == HEAT && opperation == GO){
    runMotor(heading, HEATS);
  }else if(heading = HEATS && action == LIGHTS && opperation == GO){
    runMotor(heading, LIGHTS);
  }
}

void setup(){
  pinMode(START_POT, INPUT); 
  pinMode(LIGHT_POT, INPUT); 
  pinMode(HEAT_POT, INPUT); 
  pinMode(BUTTON_POT, INPUT); 
  heading = 0;
  goTo = 0;
  servo.attach(SERVO_POT);
  heading = EEPROM.read(address);
}

void loop(){
  checkPositions();  
  checkButton();
  exicute();
}




