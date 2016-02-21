#include<SimpleServo.h>
#include<Servo.h>

//analog pins
const int ledMotionPin     = A2;

//digital pins
const int motionSensor1Pin = 2;
const int servoBasePin     = 9;
const int servoCamPin      = 10;

SimpleServo baseServo;
SimpleServo camServo;

const byte numChars = 32;
boolean newData     = false;
char receivedChars[numChars];

void setup()
{
  Serial.begin(9600);
  baseServo.attachPin(servoBasePin);
  camServo.attachPin(servoCamPin);
  baseServo.setSpeed(8);
  camServo.setSpeed(8);

  //tell pi what you have attached.
  Serial.println("BOOT_START");
   Serial.println("LED:PIRLED:A2");
   Serial.println("PIR:PIR1:2");
   Serial.println("SERVO:BASE:9");
   Serial.println("SERVO:CAM:10");
  Serial.println("BOOT_END");
}

void loop()
{
  rxData();
  if(newData == true){
     bool servoBaseFlag     = false;
     bool servoCamFlag      = false;
     bool ledMotionFlag     = false;
     bool motionSensor1Flag = false;

     char * seg = strtok (data,":");
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
              default:
                break;
             }
           }
           else{//analog pin
             if( strcmp (seg,ledMotionPin) == 0){
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

bool is_numeric(char *string)
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
