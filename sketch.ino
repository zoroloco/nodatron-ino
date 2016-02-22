#include<SimpleServo.h>
#include<Servo.h>

//analog pins
const int ledMotionPin     = A2;

//digital pins
const int motionSensor1Pin = 2;
const int powerLedPin      = 4;
const int tcpLedPin        = 7;
const int servoBasePin     = 9;
const int servoCamPin      = 10;

SimpleServo baseServo;
SimpleServo camServo;

const byte numChars = 32;
boolean newData     = false;
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
  pinMode(ledMotionPin,OUTPUT);
  pinMode(tcpLedPin,OUTPUT);

  Serial.println("BOOT_ARDUINO");
}

void loop()
{
  digitalWrite(powerLedPin,HIGH);

  rxData();
  if(newData == true){
     bool servoBaseFlag     = false;
     bool servoCamFlag      = false;
     bool ledMotionFlag     = false;
     bool tcpLedFlag        = false;
     bool motionSensor1Flag = false;

     char * seg = strtok ((char*)newData,":");
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
              case motionSensor1Pin:
                motionSensor1Flag = true;
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
          else if(motionSensor1Flag){

          }
          else if(tcpLedFlag){
            digitalWrite(tcpLedPin,atoi(seg));
          }
        }

        seg = strtok (NULL, ":");
        i++;
     }

     newData = false;//reset
  }

  //always move in loop.
  baseServo.move();
  camServo.move();
}

void rxData() {
        static byte ndx = 0;
        char endMarker = '\n';
        char rc;

    while (Serial.available() > 0 && newData == false) {
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
          newData = true;
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
