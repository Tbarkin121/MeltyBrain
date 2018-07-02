"# FOCProject" 
http://www.embedds.com/programming-stm32-discovery-using-gnu-tools-linker-script/
https://geokon-gh.github.io/stm32f4/
https://waldezlab.wordpress.com/2014/08/20/getting-started-with-the-stm32f4-discovery-ubuntu-14-04/
https://gctechspace.org/2014/09/getting-to-blinky-with-the-stm32-and-ubuntu-linux/
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
https://github.com/texane/stlink

you want to make platform-tools/lib a symlink to build-tools/20.0.0/lib, then you can either make it an absolute link:

Might be worth checking either of these out
https://github.com/adafruit/ARM-toolchain-vagrant/blob/master/Vagrantfile
https://github.com/dbaba/vagrant-openocd-stlink/blob/master/Vagrantfile



Download : 
gcc-arm-none-eabi
openocd-0.10.0
stm32cubef4
Boost (extra for now)
stlink (extra?)
stm32_discovery_arm (extra?)

Unpack zips and tar

tar -xvjf *TarFiles*

Move them to /opt/installer
chmod 777 /opt/installer
chmod 777 /opt/tools
Link directory in tools
$ ln -s /path/to/common/root/build-tools/20.0.0/lib platform-tools/lib
$ ls -l platform-tools/lib
lrwxrwxrwx 1 user user 72 Sep 16 12:57 platform-tools/lib -> /path/to/common/root/build-tools/20.0.0/lib
$ readlink -e platform-tools/lib
/path/to/common/root/build-tools/20.0.0/lib

Needed the virtual box extention pack from vagrant
needed virtual box 5.1 (5.2 will not work)

IMPORTANT!! 
sudo vi /etc/udev/rules.d/99-stlink.rules
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666"
sudo udevadm control --reload-rules

more garbage (for openocd)
sudo apt-get install libtool autoconf texinfo libusb-dev

Interesting repos
https://github.com/ObKo/stm32-cmake/blob/master/cmake/gcc_stm32f4.cmake
https://github.com/charleskorn/stm32f4-project-template
