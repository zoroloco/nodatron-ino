#include<SimpleServo.h>
#include<Servo.h>
#include <LiquidCrystal.h>

//12,11,5,4,3,2 pins reserved for LCD
const int servoBasePin = 9;
const int servoCamPin  = 10;
const int onPin        = 7;
const int actPin       = 6;

SimpleServo baseServo(servoBasePin);
SimpleServo camServo(servoCamPin);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup()
{
  Serial.begin(9600);
  //pinMode(onPin, OUTPUT);
  pinMode(actPin, OUTPUT);
  baseServo.attachPin(servoBasePin);
  camServo.attachPin(servoCamPin);
  baseServo.setSpeed(8);
  camServo.setSpeed(8);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Nodatron init...");
}

void loop()
{
  recvWithEndMarker();
  if(newData == true){
     baseServo.setAngle(atoi(receivedChars));
     camServo.setAngle(atoi(receivedChars));

     //write to LCD display the degrees of both servos.
     lcd.setCursor(0, 1);
     lcd.print(receivedChars);

     newData = false;
  }

  int baseServoMoveVal = baseServo.move();
  int camServoMoveVal  = camServo.move();

  //blink activity light if a servo is moving.
  if(baseServo.isMoving() || camServo.isMoving()){
      if(-1==baseServoMoveVal && -1==camServoMoveVal){
          digitalWrite(actPin,LOW);
      }
      else{
          digitalWrite(actPin,HIGH);
      }
  }
}

void recvWithEndMarker() {
        static byte ndx = 0;
        char endMarker = '\n';
        char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        //Serial.println(rc);//test
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

void showNewData() {
    if (newData == true) {
        Serial.println(receivedChars);
        newData = false;
    }
}
