#!/bin/sh
# Instruction: move this file to /usr/local/src and run from there.
#
# chmod +x nodatron-ino.build.sh
#
# This script will get latest site from github, move the source files 
# to their appropriate places and then build and upload the code to the 
# Arduino.
# Please run as root.
#
# Please adjust your LIB_DIR and SRC_DIR accordingly. 
#
#

LIB_DIR="/usr/share/arduino/libraries"
SRC_DIR="/usr/local/src/arduino"
TMP_DIR="/tmp/nodatron" 

clear

echo "Executing nodatron-ino build script."

echo "Deleting old tmp dir " $TMP_DIR
rm -rf $TMP_DIR

echo "Deleting old nodatron libraries directory " $LIB_DIR
rm -rf $LIB_DIR

echo "Re-creating nodatron lib directory " $LIB_DIR
mkdir $LIB_DIR

echo "Re-creating tmp dir " $TMP_DIR
mkdir $TMP_DIR

echo "Retrieving latest repo: git clone https://github.com/zoroloco/nodatron-ino.git " $TMP_DIR
git clone https://github.com/zoroloco/nodatron-ino.git $TMP_DIR

echo "Moving latest sketch file to " $SRC_DIR/src
cp $TMP_DIR/sketch.ino $SRC_DIR/src

echo "Updating libraries in " $LIB_DIR
cp -R $TMP_DIR/usr/share/arduino/libraries/* $LIB_DIR

echo "Cleaning up /tmp"
rm -rf $TMP_DIR

echo "Now building latest Nodatron."
cd $SRC_DIR
ino build
sleep 10
echo "Uploading Nodatron to Arduino."
ino upload

echo "All Done! Good Nodatron!"

