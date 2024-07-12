# Oxyz3d
An 3d editor  - C++/OpengL/Fltk



sudo apt-get install freeglut3
sudo apt-get install freeglut3-dev

sudo apt-get install liblua5.4-dev

sudo apt-get install libxinerama-dev
sudo apt-get install libxfixes-dev
sudo apt-get install libpng-dev
sudo apt-get install libjpeg-dev

sudo apt-get install libxrender1
sudo apt-get install libxrudender-dev

sudo apt-get install libxcursor1
sudo apt-get install libxcursor-dev

sudo apt-get install libfontconfig1
sudo apt-get install libfontconfig1-dev

//=================
dpkg -L libX11-dev



//=================
gtk !
sudo apt install libgtk-4-dev
sudo apt install libgtkmm-3.0-dev


pkg-config --cflags gtk4
pkg-config --libs gtk4
cc `pkg-config --cflags gtk4` hello.c -o hello `pkg-config --libs gtk4`
gtkmm