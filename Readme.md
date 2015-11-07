Server Pic
=======
This project is used for **processing picture on linux server** (using **c++** as main language)  

Before using this program, you should configurate the environment according to below:
  * git  
  * opencv and depend libs  
  * json-c  
  * qt-creator (recommend)  

Install git (ubuntu)
-------
`$ sudo apt-get install git`  

Install opencv (ubuntu)
-------
`$ sudo apt-get install build-essential`  
`$ sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev`  

donwload opencv for linux from http://opencv.org/downloads.html  

`$ cmake .`  
`$ sudo make`  
`$ sudo make install`  
`$ sudo gedit /etc/ld.so.conf.d/opencv.conf`  

**add below to tail of file**  
`/usr/local/lib`  

`$ sudo ldconfig`  
`$ sudo gedit /etc/bash.bashrc`  

**add below to tail of file**  
`PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig`  
`export PKG_CONFIG_PATH`  

`$ sudo source /etc/bash.bashrc`  
`$ sudo updatedb`  

Install json-c (ubuntu)   
-------
**json-c official github** : https://github.com/json-c/json-c  
`$ git clone https://github.com/json-c/json-c.git`  
`$ cd json-c`  
`$ sh autogen.sh`  

followed by  
`$ ./configure`  
`$ make`  
`$ make install`  

Install libcurl (ubuntu)   
-------
**libcurl official download page** : http://curl.haxx.se/download.html  

after download, followed by  
`$ ./configure`  
`$ make`  
`$ sudo make install`  

use these command to get include/lib  
`$ curl-config --cflags`  
`$ curl-config --libs`  
