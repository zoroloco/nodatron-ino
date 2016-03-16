# raspatron.

1.) Move update-raspatron.sh to /usr/local/src
2.) Run update-raspatron.sh
3.) You now have latest code.

OR

git clone https://github.com/zoroloco/raspatron.git

#about

This is a robot that utilizes the Nodatron framework.

It has 2 hi-tech 311 servos that pan and tilt a camera.  The camera
is a raspberry pi camera module connected to a raspberry pi.
The pi will be running this code that controls the Arduino components.

The robot will have 3 sensors.  Left, Right and front.

If a sensor is triggered, then the camera will move to face the sensor and the motion
led will blink.

The robot also has an on/off button.  If off, then the sensors are disabled, the
streaming feed from the camera is turned off and all leds are turned off.

The robot can also accept a TCP connection.  When a remote connection is made, the
blue LED will turn on.

Whenever there is activity on the robot, the yellow activity LED will blink.

If you press the power button and it was previously off, then the camera will tilt upwards
(wake up) and the sensors will be enabled again.  The video stream will also be started again.
The red LED power light will also turn on.

#remote control it

Download and run the associated IOS app called raspatron-app.

https://github.com/zoroloco/raspatron-app


Note:  You will need to open a port for the video streaming and another port if you want
to accept remote TCP connections.
