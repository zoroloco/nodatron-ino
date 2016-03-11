#include<SimpleServo.h>
#include<Servo.h>

//digital pins

//leds
const int powerLedPin     = 2;
const int activityLedPin  = 3;
const int motionLedPin    = 4;
const int tcpLedPin       = 5;

//buttons
const int powerButtonPin  = 6;

//sensors
const int pirFrontPin     = 7;
const int pirLeftPin      = 8;
const int pirRightPin     = 11;

//servos
const int servoBasePin    = 9;
const int servoCamPin     = 10;

SimpleServo baseServo;
SimpleServo camServo;

//globals
int pirFrontState        = LOW;
int pirLeftState         = LOW;
int pirRightState        = LOW;
int powerButtonState     = 0;
bool powerButtonPressed  = false;
const byte numChars      = 32;
bool  newDataFlag        = false;
char receivedChars[numChars];

void setup()
{
  //serial setup
  Serial.begin(9600);

  //servo setup
  baseServo.attachPin(servoBasePin);
  camServo.attachPin(servoCamPin);
  baseServo.setSpeed(8);
  camServo.setSpeed(8);

  //led setup
  pinMode(powerLedPin,OUTPUT);
  pinMode(activityLedPin,OUTPUT);
  pinMode(motionLedPin,OUTPUT);
  pinMode(tcpLedPin,OUTPUT);

  //sensor setup
  pinMode(pirFrontPin,INPUT);
  pinMode(pirLeftPin,INPUT);
  pinMode(pirRightPin,INPUT);

  //button setup
  pinMode(powerButtonPin,INPUT);
}

void loop()
{
  rxData();
  if(newDataFlag){
     bool servoBaseFlag     = false;
     bool servoCamFlag      = false;

     bool powerLedFlag      = false;
     bool activityLedFlag   = false;
     bool motionLedFlag     = false;
     bool tcpLedFlag        = false;

     char * seg = strtok(receivedChars,":");
     int i = 0;

     while (seg != NULL)
     {
       if(i==0){//what pin?
           if(isNumeric(seg)){
             switch(atoi(seg)){
              case servoBasePin:
                servoBaseFlag = true;
                break;
              case servoCamPin:
                servoCamFlag = true;
                break;
              case powerLedPin:
                powerLedFlag = true;
                break;
              case activityLedPin:
                activityLedFlag = true;
                break;
              case motionLedPin:
                motionLedFlag = true;
                break;
              case tcpLedPin:
                tcpLedFlag = true;
              default:
                break;
             }
           }
           else{//analog pin
             if( strcmp (seg,"A0") == 0){

             }
             else if( strcmp (seg, "A1") == 0){

             }
             else if( strcmp (seg, "A2") == 0){

             }
             else if( strcmp (seg, "A3") == 0){

             }
             else if( strcmp (seg, "A4") == 0){

             }
             else if( strcmp (seg, "A5") == 0){

             }
           }
        }
        else if(i==1){//do something
          if(servoBaseFlag){
            baseServo.setAngle(atoi(seg));
          }
          else if(servoCamFlag){
            camServo.setAngle(atoi(seg));
          }
          else if(powerLedFlag){
            digitalWrite(powerLedPin,atoi(seg));
          }
          else if(activityLedFlag){
            digitalWrite(activityLedPin,atoi(seg));
          }
          else if(motionLedFlag){
            digitalWrite(motionLedPin,atoi(seg));
          }
          else if(tcpLedFlag){
            digitalWrite(tcpLedPin,atoi(seg));
          }
        }

        seg = strtok (NULL, ":");
        i++;
     }

     newDataFlag = false;//reset
  }

  detectPowerButton();
  detectFrontMotion();
  detectLeftMotion();
  detectRightMotion();
  baseServo.move();
  camServo.move();
}

void detectPowerButton(){
  if(digitalRead(powerButtonPin)){
    powerButtonPressed = true;
  }
  else{
    if(powerButtonPressed){
      powerButtonPressed = false;

      if(powerButtonState){
        Serial.println("{powerButton:0}");
        powerButtonState = 0;
      }
      else{
        Serial.println("{powerButton:1}");
        powerButtonState = 1;
      }
    }
  }
}

void detectFrontMotion(){
  if(digitalRead(pirFrontPin)){
    if(pirFrontState==LOW){
      Serial.println("{pirSensorFront:1}");
      pirFrontState = HIGH;
    }
  }
  else{
    if(pirFrontState==HIGH){
      Serial.println("{pirSensorFront:0}");
      pirFrontState = LOW;
    }
  }
}

void detectLeftMotion(){
  if(digitalRead(pirLeftPin)){
    if(pirLeftState==LOW){
      Serial.println("{pirSensorLeft:1}");
      pirLeftState = HIGH;
    }
  }
  else{
    if(pirLeftState==HIGH){
      Serial.println("{pirSensorLeft:0}");
      pirLeftState = LOW;
    }
  }
}

void detectRightMotion(){
  if(digitalRead(pirRightPin)){
    if(pirRightState==LOW){
      Serial.println("{pirSensorRight:1}");
      pirRightState = HIGH;
    }
  }
  else{
    if(pirRightState==HIGH){
      Serial.println("{pirSensorRight:0}");
      pirRightState = LOW;
    }
  }
}

void rxData() {
        static byte ndx = 0;
        char endMarker = '\n';
        char rc;

    while (Serial.available() > 0 && newDataFlag == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
          receivedChars[ndx] = '\0'; // terminate the string
          ndx = 0;
          newDataFlag = true;
        }
    }
}

bool isNumeric(char *string)
{
    int sizeOfString = strlen(string);
    int iteration = 0;
    bool isNumeric = true;

    while(iteration < sizeOfString)
    {
        if(!isdigit(string[iteration]))
        {
            isNumeric = false;
            break;
        }

        iteration++;

    }

    return isNumeric;
}
