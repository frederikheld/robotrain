#!/bin/sh

# Run this script to setup your robotrain raspi-io

#
#   INSTALL REQUIRED TOOLS

# Git:
sudo apt-get install git

# WiringPi (To access GPIO. Also prerequisite for node.js package raspi-io):
# See: https://github.com/nebrius/raspi-io
sudo apt-get install wiringpi


#
#   INSTALL NODE.JS
#

# Remove legacy node.js (which might be pre-installed and will prevent the installation of the new version):
sudo apt-get uninstall nodejs-legacy
# See: https://www.packtpub.com/books/content/programming-raspberry-pi-robots-javascript

# Install gcc/g++ 4.8:
sudo apt-get update
sudo apt-get install gcc-4.8 g++-4.8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 20
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.6 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50

# Download and install node_arm:
wget http://node-arm.herokuapp.com/node_latest_armhf.deb
sudo dpkg -i node_latest_armhf.deb

# Check installation
# node -v

# See: http://node-arm.herokuapp.com/

# Install node packet manager:
sudo apt-get install npm
