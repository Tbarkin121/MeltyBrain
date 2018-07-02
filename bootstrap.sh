#!/usr/bin/env bash

#dpkg -L <packagename>

#cd /vagrant/tools
#git clone https://github.com/texane/stlink.git
#git clone https://github.com/rowol/stm32_discovery_arm_gcc

apt-get update
sudo apt-get install -y lib32z1 lib32ncurses5
sudo apt-get install autoconf pkg-config libusb-1.0 git libusb-dev pkg-config libtool
sudo apt-get install -y build-essential autotools-dev autoconf pkg-config libusb-1.0-0 libusb-1.0-0-dev libftdi1 libftdi-dev git libc6:i386 libncurses5:i386 libstdc++6:i386
apt-get install -y cmake
apt-get install -y build-essential
#apt-get install -y openocd
#apt-get install -y gcc-arm-none-eabi
#apt-get install -y gdb-arm-none-eabi
#apt-get -y install lib32z1 lib32ncurses5 lib32bz2-1.0
