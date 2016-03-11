#!/bin/sh
# Grabs latest code from github, builds and executes.
#
# note: this script does NOT update the Arduino custom libraries. Move those yourself.

SRC_DIR = "/usr/local/src"
TMP_DIR = "/tmp/raspatron"

clear

echo "Executing update script..."

echo "Deleting old tmp dir" $TMP_DIR
rm -rf $TMP_DIR

echo "Deleting old raspatron dir"
rm -rf $SRC_DIR/raspatron

echo "Re-creating tmp dir " $TMP_DIR
mkdir $TMP_DIR

echo "Retrieving latest repo: git clone https://github.com/zoroloco/raspatron.git " $TMP_DIR
git clone https://github.com/zoroloco/raspatron.git $TMP_DIR

echo "Moving latest code to " $SRC_DIR
cp -R $TMP_DIR $SRC_DIR

echo "Cleaning up " $TMP_DIR
rm -rf $TMP_DIR

echo "Now building latest Raspatron."
cd $SRC_DIR/raspatron/arduino
ino build
sleep 5
echo "Uploading Raspatron to Arduino."
ino upload

echo "Now installing dependencies."
cd $SRC_DIR/raspatron
sudo npm install

echo "Now running raspatron."
sudo npm start
