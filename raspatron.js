/**
 * A robot that uses a maestro and raspberry pi to control its GPIO and
 * multiple servos.
 *
*/
var nodatron = require('./node_modules/nodatron/lib/nodatron.js'),
    pathUtil = require('path'),
    _        = require('underscore'),
    log      = require(pathUtil.join(__dirname,'./logger.js'))

    var maestro = new nodatron({"device" : "maestro"});
    var piZero  = new nodatron({"device" : "pizero"});

    maestro.enableConsole();

    maestro.on("connected", function(){
      var camPanServo = maestro.createServo(0);
      var camTiltServo= maestro.createServo(1);

      camPanServo.move(0);
      camTiltServo.move(0);      
    }
