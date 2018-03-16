libQtShadowsocks
================

[![Build Status](https://travis-ci.org/shadowsocks/libQtShadowsocks.svg?branch=master)](https://travis-ci.org/shadowsocks/libQtShadowsocks) <a href="https://copr.fedorainfracloud.org/coprs/librehat/shadowsocks/package/libQtShadowsocks/"><img src="https://copr.fedorainfracloud.org/coprs/librehat/shadowsocks/package/libQtShadowsocks/status_image/last_build.png" /></a>

Introduction
------------

`libQtShadowsocks` is a lightweight [shadowsocks][ss] library.

Written in C++ using Qt 5 framework and Botan library, it aims to provide a developer-friendly [shadowsocks][ss] library for Qt applications such as [shadowsocks-qt5](https://github.com/shadowsocks/shadowsocks-qt5/).

The sub-project `shadowsocks-libqss` utilises this library. You may wish to check the code to see how to integrate `libQtShadowsocks` into your project. `shadowsocks-libqss` can also be used as a standalone program in both local-side and server-side.

Check [installation wiki page](https://github.com/shadowsocks/libQtShadowsocks/wiki/Installation) to see how to install this library and/or `shadowsocks-libqss`.

[ss]: http://shadowsocks.org

Compilation Instructions for Windows
-------

* requirements: Visual Studio 2015 Community Edition, Python 3.6+, Qt 5.10.1, pkg-config for Windows, cmake 3.9+

* compile Botan library:

  - download Botan library sources from https://botan.randombit.net

  - make sure that Python is in your path environment variable

  - open a Command Window and setup environment variables for Visual Studio 2015 by running

    "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

  - in the Command Window change the current folder to Botan library sources (make sure that the sources path does not contain spaces)

  - in order to compile and install Botan library use:

    configure.py --cc=msvc --os=windows
    nmake
    nmake install

  - at the end of the installation process a folder Botan is created on the current drive (e.g. C:\Botan)

  - inside Botan folder, there should be a lib folder. Create inside the lib folder another folder, pkgconfig, and put there botan.pc file. If needed, edit this file in order to correct the paths to lib and include folders.

* setup pkg-config in order to detect the installed Botan libray:

  - download pkg-config for Windows and its dependences (https://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows)

  - make sure that the path to pkg-config.exe is in your PATH environment variable

  - create an environment variable, PKG_CONFIG_PATH, that points to the location of botan.pc file you created above (e.g. C:\Botan\lib\pkgconfig)

  - open a Command Window and type
    pkg-config --list-all
  in order to make sure that Botan library is detected.

* compile QtShadowsocks:

  - make sure that the path to cmake.exe is in your PATH environment variable

  - open with QtCreator CMakeLists.txt file and and run cmake, then build the entire project using "Release with Debug Information" build option

  - in order to run the resulting binary make sure that the path to Qt DLLs is in your PATH environment variable (e.g. C:\Qt\5.10.1\msvc2015_64\bin)

Request at runtime Administrator privileges
-------

This step is required if you want to automatically add Administrator privileges when starting the application.

- open a Command Window and setup environment variables for Visual Studio 2015 by running

  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

- copy shadowsocks-libqss.exe.manifest file next to the generated shadowsocks-libqss.exe file, then from the folder that contains both files run

  mt -manifest shadowsocks-libqss.exe.manifest -outputresource:"shadowsocks-libqss.exe";#1

Running as Windows Service
-------

- you must copy next to the service executable all required dependences (DLLs). In order to do so you must manually copy botan.dll and all required Qt libraries running the command below from the service executable folder:
    C:\Qt\5.10.1\msvc2015_64\bin\windeployqt.exe --release shadowsocks-libqss.exe

- also remember to pass only absolute paths as command line options when starting the service

- the service acts as a wrapper around the application and adds new command line options accessible when the application is started with:
  shadowsocks-libqss.exe -h

- the application can still be started normally using "-e" option, followed by any number of options. For example, in order to show the original command line options use:
  shadowsocks-libqss.exe -e --help
The same original options can be used also when starting the application as service.

- in order to run the application as service, the service needs to be installed first with (you must start as Administrator the terminal):
  shadowsocks-libqss.exe -i

- in order to check the service status use (you must start as Administrator the terminal):
  shadowsocks-libqss.exe -v

- in order to start the service (you must start as Administrator the terminal):
  shadowsocks-libqss.exe --config-file C:\config.json -f c:\shadowsocks.log
Note that you can still use the options for the standalone application, but for the service the specified paths need to be absolute.

- in order to stop the service (you must start as Administrator the terminal):
  shadowsocks-libqss.exe -t

- in order to uninstall the service (you must start as Administrator the terminal):
  shadowsocks-libqss.exe -u

License
-------

![](http://www.gnu.org/graphics/lgplv3-147x51.png)

Copyright (C) 2014-2017 Symeon Huang

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library. If not, see <http://www.gnu.org/licenses/>.
