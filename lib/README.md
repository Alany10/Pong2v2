# Getting started to program with SDL2 in C

- [Getting started to program with SDL2 in C](#getting-started-to-program-with-sdl2-in-c)
  - [Installing SDL2 Libraries](#installing-sdl2-libraries)
    - [Ubuntu Linux](#ubuntu-linux)
    - [SDL2 on Macos](#sdl2-on-macos)
    - [SDL2 on Windows](#sdl2-on-windows)
  - [Running the hello program](#running-the-hello-program)
  - [Bonus](#bonus)

This is a simple tutorial to install and run a simple program which uses the _Simple Direct Media Library version 2 or SDL2_ in a C program.

This tutorial assumes that you have installed and are familiar with running a C program as shown in the tutorial [here](https://github.com/Makerspace-KTH/c_programing_intro)

To summarize, the tutorial assumes that:

1. you are using either Windows (10 or 11), Ubuntu Linux, or Macos.
2. You have installed GNU C compiler (GCC) using the method described in the tutorial for your operating system. 
3. You are able able to successfully compile the a program using make.

A program with SDL2 libraries will not only require you to include and call the functions in the SDL2 library but also to provide the runtime libraries during the _linking_ process. The following sections describe how to install a SDL2 library and to tell the compiler to use it and link your program against it, to create the executable that can be run on your machine.

## Installing SDL2 Libraries

SDL2 contains various libraries based on what type of functions are called. For example, the _SDL2\_mixer_ library contains functions to manage sound when creating games, while _SDL2\_ttf_ library helps in creating fonts. The libraries are separated based on their functions but also to ensure that only the required parts of the libraries are used for building the executable. The size of the library will have a influence on the size of the executable cerated.

To install the SDL2 libraries follow the instructions as per the operating system:

### Ubuntu Linux

On Ubuntu linux, use the following command in the terminal to install the required libraries:

    $ sudo apt install -y libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-ttf-2.0-0 libsdl2-net-2.0-0 libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-mixer-dev

### SDL2 on Macos

On mac with _homebrew_ installed please use the following command in terminal to install the libraries:

    $ brew install sdl2 sdl2_mixer sdl2_image sdl2_ttf sdl2_net

### SDL2 on Windows

On widows with Msys2, please open the msys2 console and issue the following commands to install the SDL2 libraries.

    $ pacman -S mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_net

Answer _Y_ to proceed with the installation.

## Running the hello program

In order to run the program using Makefile, rename the right makefile for you system. For example, on a Windows computer, rename _Makefile\_win_ to _Makefile_. 

Open a terminal and point the working directory to the project folder. Refer to the [_c\_programming\_intro_](https://github.com/Makerspace-KTH/c_programing_intro#4-launch-and-run-a-c-program) for details for your operating system.

Run the makefile based on your OS to build the _hello_ executable

Windows:

    $ mingw32-make.exe

Linux / macos:

    $ make

Execute the binary _hello_

    $ ./hello 

or

    $ ./hello.exe

Expected output:

![Expected Output](contents/expected-output.png?raw=true "Expected output")

Run

$ make clean

to remove the binaries.

## Bonus

Try to download and compile a full alien game from [here](https://github.com/Makerspace-KTH/alien).
