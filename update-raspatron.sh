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

echo "Now running raspatron."
sudo npm start
