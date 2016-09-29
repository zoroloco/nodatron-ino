/*
This file is an example on how to use the Nodatron framework to control
servos, leds, etc on an arduino. Please see the arduino folder to see how to
write the arduino code that receives the messages.

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

Note: Replace the file raspatron.js with this file.
*/

var nodatron = require('./node_modules/nodatron/lib/nodatron.js'),
    pathUtil = require('path'),
    _        = require('underscore'),
    log      = require(pathUtil.join(__dirname,'./logger.js')),
    tcpServer= require(pathUtil.join(__dirname,'./tcpServer.js')),
    cp       = require('child_process');

var arduino = new nodatron({"device"  : "arduino",
                            "devPath" : "/dev/ttyACM0",
                            "baud"    : 9600});

var server  = new tcpServer(8170);

arduino.enableConsole();

arduino.on("connected", function(){
  var powerLed          = arduino.createLed(2);
  var activityLed       = arduino.createLed(3);
  var motionLed         = arduino.createLed(4);
  var connectivityLed   = arduino.createLed(5);
  var powerButton       = arduino.createButton(6);
  var motionSensorFront = arduino.createPir(7);
  var motionSensorLeft  = arduino.createPir(8);
  var motionSensorRight = arduino.createPir(11);
  var baseServo         = arduino.createServo(9);
  var camServo          = arduino.createServo(10);
  var speechModule      = arduino.createEmicToSpeech(12);

  server.connect(
    function(remoteIP){//connected
      log.info("Client has connected from remote IP address:"+remoteIP);
      speechModule.speak("Remote client has connected.");
      connectivityLed.turnOn();
    },
    function(data){//got data
      log.info("TCP server received data from a client."+data);

      //convert buffer to string
      var dataStr = data.toString('utf8');

      //TODO: do proper parsing and message handling.
      //lets just get the servos back working again remotely.
      var splitData = dataStr.split(":");
      if(!_.isEmpty(splitData[0])){
        if(splitData[0] == "9"){
          baseServo.move(splitData[1]);
        }
        else if(splitData[0] == "10"){
          camServo.move(splitData[1]);
        }
      }
    },
    function(){//error
      log.error("TCP server had an error.");
      speechModule.speak("Remote client has disconnected.");
      connectivityLed.turnOff();
    });

  powerButton.on('on',function(){
    speechModule.speak("Raspatron powering on.");
    powerLed.turnOn();
    panCenter();

  });

  powerButton.on('off',function(){
    speechModule.speak("Raspatron powering off.");
    powerLed.turnOff();
    motionLed.turnOff();
    connectivityLed.turnOff();
    activityLed.turnOff();
    sleepCam();
  });

  motionSensorFront.on('start',function(){
    if(powerButton.isOn()){
      motionLed.startBlink();
      panCenter();
    }
  });

  motionSensorFront.on('stop',function(){
    motionLed.stopBlink();
  });

  motionSensorLeft.on('start',function(){
    if(powerButton.isOn()){
      motionLed.startBlink();
      panLeft();
    }
  });

  motionSensorLeft.on('stop',function(){
  motionLed.stopBlink();
  });

  motionSensorRight.on('start',function(){
    if(powerButton.isOn()){
      motionLed.startBlink();
      panRight();
    }
  });

  motionSensorRight.on('stop',function(){
    motionLed.stopBlink();
  });

  function panLeft(){
    baseServo.move(180);
    camServo.move(85);
  }

  function panRight(){
    baseServo.move(33);
    camServo.move(85);
  }

  function panCenter(){
    baseServo.move(110);
    camServo.move(85);
  }

  //move camera to a rest position.
  function sleepCam(){
    baseServo.move(110);
    camServo.move(0);
  }

  function executeCommand(cmd,cb){
    var child = cp.exec(cmd ,function(error,stdout,stderr){
      if (stderr){
        cb(false,"Error executing command "+cmd+" with stderr:"+stderr);
      }
      if (error) {
        cb(false,"Error executing command "+cmd+" with error:"+error);
      }
      if(stdout){
        cb(true,cmd+" successfully executed with no errors.",stdout);
      }
    });
  }


  //define handlers for this main process.
  process.on('SIGTERM', function() {//called from /etc/init.d/nodatron.sh from kill pid
    log.info("Got kill signal. Exiting.");
    shutdown();
  });

  process.on('SIGINT', function() {
    log.warn("Caught interrupt signal(Ctrl-C)");
    shutdown();
  });

  process.on('exit', function(){
    log.info("raspatron exiting...");
  })

  process.on('uncaughtException', function (err) {
    var msg="Uncaught Exception ";
    if( err.name === 'AssertionError' ) {
      msg += err.message;
    } else {
      msg += err;
    }
    log.error(msg);
    shutdown();
  });

  function shutdown(){
    arduino.shutdown();
    log.info("Arduino serial connection shut down.");

    server.shutdown(function(){
      log.info("TCP server shut down.");
    });

    process.exit();
  }

});
