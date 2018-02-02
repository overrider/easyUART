# easyUART
An easy to use library to talk to a UART device under linux

## Introduction
This library was designed to connect with a UART device under /dev/ttyUSB0 and exchange data. 
In theory, all you have to do is download it, configure your UART device in app.h , and run make.

The program uses Linux signals to receive data so you are free to do whatever you like inside your 
main while loop. It is non-blocking.

Incoming data is read until \r is received, upon which the whole string gets added to an incoming buffer. 
From there, the main while loop will pick one Item in this buffer process and remove. 

The same goes for outgoing. To send a command to the UART you call add_to_outgoing("Command"). This
will add the command to an outgoing buffer. The main while loop will now, on each iteration, pick
one outgong item and send it to the UART.

## Configuration
Adapt app.h to set your device and baudrate.
If you don't have a UART device such as an Arduino to test with, you can emulate one using socat:
socat -d -d pty,raw,echo=0 pty,raw,echo=0

## Compilation
1. cd easyUart
2. make

Running ./make will result in uart_app. I developed and tested this under Debian Linux (Jessie) but i guess it will
work fine for any common Distro.
