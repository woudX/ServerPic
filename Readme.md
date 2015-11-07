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

使用说明
======= 
启动服务器
--------------
部署好上述环境后，按照以下步骤操作即可：  
  * 在Build/目录下找到ServerPic，并拷贝到想要放置的位置  
  * 找到配置文件Build/config.json，修改其中的参数，并放置到能通过http访问的位置上
  * 运行ServicePic，需要添加配置文件的URL参数信息，配置文件在Build/config.json处以供参考，一般运行命令如下  
`$ ./ServerPic http://192.168.89.1/Test/config.json`  

指令说明
--------------
服务器启动后，需要通过socket接口进行访问，本程序目前支持如下访问指令：  
**formal,[arg1],[arg2]**：主要流程命令，负责解压缩到完成图片整个处理过程，并向服务器发出json字符串，参数说明：  
  * [arg1]:zip的相对或者绝对路径  
  * [arg2]:该zip所对应的工程号  
`Demo command : formal,Desktop.zip,15`  
  
**exit**:命令服务器停止运行的指令
