#include<SimpleServo.h>
#include<Servo.h>

//analog pins
const int ledMotionPin     = A2;

//digital pins
const int pirPin          = 2;
const int streamButtonPin = 4;
const int tcpLedPin       = 7;
const int servoBasePin    = 9;
const int servoCamPin     = 10;

SimpleServo baseServo;
SimpleServo camServo;

//globals
int pirState             = LOW;
int streamButtonState    = 0;
bool streamButtonPressed = false;
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
  pinMode(ledMotionPin,OUTPUT);
  pinMode(tcpLedPin,OUTPUT);

  //sensor setup
  pinMode(pirPin,INPUT);

  //button setup
  pinMode(streamButtonPin,INPUT);

  Serial.println("{Z}");
}

void loop()
{
  rxData();
  if(newDataFlag){
     bool servoBaseFlag     = false;
     bool servoCamFlag      = false;
     bool ledMotionFlag     = false;
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
              case tcpLedPin:
                tcpLedFlag = true;
                break;
              default:
                break;
             }
           }
           else{//analog pin
             if( strcmp (seg,"A2") == 0){
               ledMotionFlag = true;
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
          else if(ledMotionFlag){
            digitalWrite(ledMotionPin,atoi(seg));
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

  detectStreamButton();
  detectMotion();
  baseServo.move();
  camServo.move();
}

void detectStreamButton(){
  if(digitalRead(streamButtonPin)){
    streamButtonPressed = true;
  }
  else{
    if(streamButtonPressed){
      streamButtonPressed = false;

      if(streamButtonState){
        Serial.println("{streamButton:0}");
        streamButtonState = 0;
      }
      else{
        Serial.println("{streamButton:1}");
        streamButtonState = 1;
      }
    }
  }
}

void detectMotion(){
  if(digitalRead(pirPin)){
    if(pirState==LOW){
      Serial.println("{pirSensor:1}");
      pirState = HIGH;
    }
  }
  else{
    if(pirState==HIGH){
      Serial.println("{pirSensor:0}");
      pirState = LOW;
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
