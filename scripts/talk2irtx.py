#!/usr/bin/env python
#
# File Name: talk2irtx.py
#
# Purpose: A program to control 4642HD NextBox using arduino based IR transmitter
#
#  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
#  MIT Licence (See LICENSE.txt for details) 

import sys, getopt, os
import serial
import fcntl
import subprocess
import time

# Equivalent of the _IO('U', 20) constant in the linux kernel.
USBDEVFS_RESET = ord('U') << (4*2) | 20

verbose = False

def version():
    print os.path.basename( __file__ ), '-', Description,
    print '( Version:', Version, Author, Date,')'
    sys.exit()


def usage():
    print 'usage: ', os.path.basename( __file__ ), '"command"'
    print '       -h        usage information (this output)'
    print '       -R        reset USB port that Arduino is connected to'
    print '       -v        output version information'
    sys.exit()


def getArduinoUsbDevice():
    proc = subprocess.Popen(['lsusb'], stdout=subprocess.PIPE)
    out = proc.communicate()[0]
    lines = out.split('\n')
    for line in lines:
        if 'Arduino' in line:
            parts = line.split()
            bus = parts[1]
            dev = parts[3][:3]
            print '/dev/bus/usb/%s/%s' % (bus, dev)
            return '/dev/bus/usb/%s/%s' % (bus, dev)


def send_reset(dev_path):
    fd = os.open(dev_path, os.O_WRONLY)
    try:
        fcntl.ioctl(fd, USBDEVFS_RESET, 0)
    finally:
        os.close(fd)


def resetUsbPort():
    print "reset USB port"
    send_reset(getArduinoUsbDevice())
    sys.exit()


def main(argv):

    try:
        opts, args = getopt.getopt(argv,"hRv",["help", "Reset", "version"])
    except getopt.GetoptError:
        usage()

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-R", "--reset"):
            resetUsbPort()
        elif opt in ("-v", "--version"):
            version()
   
    if len(args) == 0:    
        print '**ERROR** Command not specified! Use \'-\' for stdin. (Type \'',\
            os.path.basename( __file__ ), '-h\' for help.)'
        sys.exit()

    #Reset serial port
    s = serial.Serial('/dev/ttyACM0')

    with s:
        s.setDTR(False)
        time.sleep(1)
        s.flushInput()
        s.setDTR(True)


    #Configure to serial port
    s = serial.Serial('/dev/ttyACM0',
                      baudrate=115200,
                      bytesize=serial.EIGHTBITS,
#                      parity=serial.PARITY_NONE,
#                      stopbits=serial.STOPBITS_ONE,
#                      xonxoff=0,
                      timeout=1,
#                      rtscts=0
                      )
    
    s.write("*IDN?\n")
    time.sleep(1)
    response = s.readline()
    print response
    count=0
    while "*IDN Arduino - Cisco 4642HD Remote Control" not in response:
        print "resend *IDN?"
        s.write("*IDN?\n")
        response = s.readline()
        print response
        time.sleep(1)
        count += 1
        if count > 10:
            break
 
    for command in args:
        print "Processing :'" + command + "'"    
        s.write(command + "\n")
        time.sleep(4)
        response = s.readline()
        print response

    print "Done!"        
    sys.exit()

if __name__ == "__main__":
    main(sys.argv[1:])
