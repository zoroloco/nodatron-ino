Installation instructions for raspatron.

apt-get update
apt-get upgrade

download latest node js soure and compile. (4.3+)


install npm
install python 2

cd /usr/local/src

chmod +x raspatron.sh

Copy the start up script located in the dir: rpi/etc/init.d/raspatron.sh to /etc/init.d

chmod +x /etc/init.d/raspatron.sh

sudo update-rc.d raspatron.sh defaults (will add this to startup script)

#now set time for an accurate log file time stamp.
sudo raspi-config
2. Internationalization options
3. Change Time Zone
4. Select geographical area
5. Select city or region.
6. Reboot your pi.

install mjpg-streamer
http://blog.miguelgrinberg.com/post/how-to-build-and-run-mjpg-streamer-on-the-raspberry-pi

now run with:  /etc/init.d/raspatron.sh start
stop    with:  /etc/init.d/raspatron.sh stop
