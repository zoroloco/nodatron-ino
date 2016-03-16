#!/bin/sh
# Grabs latest code from github, builds and executes.
#
# note: this script does NOT update the Arduino custom libraries. Move those yourself.

clear

echo "Executing update script..."

rm -rf /usr/local/src/raspatron
git clone https://github.com/zoroloco/raspatron.git /usr/local/src/raspatron

echo "Now building latest Raspatron."
cd /usr/local/src/raspatron/arduino
ino build
#sleep 5
#echo "Uploading Raspatron to Arduino."
#ino upload

echo "Now installing dependencies."
cd /usr/local/src/raspatron
sudo npm install

echo "making scripts executable"
chmod +x /usr/local/src/raspatron/startStream.sh
chmod +x /usr/local/src/raspatron/stopStream.sh
chmod +x /usr/local/src/raspatron/raspatron.sh

echo "moving and updating startup script"
sudo cp /usr/local/src/raspatron/rpi/etc/init.d/raspatron.sh /etc/init.d
sudo chmod +x /etc/init.d/raspatron.sh
sudo update.rc.d /etc/init.d/raspatron.sh defaults

echo "Now running raspatron."
sudo npm start
