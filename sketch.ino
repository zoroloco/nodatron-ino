#include<SimpleServo.h>
#include<Servo.h>

const int servoBasePin = 9;
const int servoCamPin  = 10;

SimpleServo baseServo();
SimpleServo camServo();

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
}

void loop()
{
  rxData();
  if(newData == true){
     baseServo.processData(receivedChars);
     camServo.processData(receivedChars);
     newData = false;
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
