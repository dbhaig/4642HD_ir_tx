#!/usr/bin/env python
#
#  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
#  MIT Licence (See LICENSE.txt for details)
#
# Created: 17-01-2016
# Last Modified: Tue 22 Nov 2016 10:29:29 PM EST -0500

""" Control 4642HD NextBox using arduino based IR transmitter """

import sys
import os
import serial
import subprocess
import time
import os.path
from optparse import OptionParser

PATH = "/dev/ir_tx"


def arduino_uno_connected():
    """ Check for Arduino UNO hardware """
    if os.path.islink(PATH):
        return True
    else:
        proc = subprocess.Popen(['lsusb'], stdout=subprocess.PIPE)
        out = proc.communicate()[0]
        lines = out.split('\n')
        for line in lines:
            if 'ID 2341:0001 Arduino SA Uno (CDC ACM)' in line:
                output_profile_info()
            if 'ID 1a86:7523 QinHeng Electronics HL-340 USB-Serial' in line:
                output_profile_info()
    print "*** ERROR *** Arduino Uno NOT found!"
    sys.exit()


def output_profile_info():
    print '# *** Info *** Device path NOT defined.'
    print '# Use lsusb to get Vid and Pid for the Arduino board you are using'
    print '# Add a line to /etc/udev/rules.d/50-usb.rules'\
          + ' and reboot the system'
    print '# SUBSYSTEM=="tty", ATTRS{idVendor}=="Vendor ID",'\
          + ' ATTRS{idProduct}=="<Product ID>", SYMLINK+="ir_tx",'\
          + ' MODE="0666"'
    print "# Reboot to load rules and create symlink"

    sys.exit(1)


def configure_serial_port():
    # Reset serial port
    s_p = serial.Serial(PATH)

    with s_p:
        s_p.setDTR(False)
        time.sleep(1)
        s_p.flushInput()
        s_p.setDTR(True)

    # Configure to serial port
    s_p = serial.Serial(PATH,
                        baudrate=115200,
                        bytesize=serial.EIGHTBITS,
                        # parity=serial.PARITY_NONE,
                        # stopbits=serial.STOPBITS_ONE,
                        # xonxoff=0,
                        timeout=1,
                        # rtscts=0
                        )
    return s_p


def arduino_is_an_ir_tx(port):
    for trys in range(0, 2):   # try three times if necessary
        port.write("*IDN?\n")
        time.sleep(1)
        response = port.readline()
        if response == "*IDN Arduino - Cisco 4642HD Remote Control\r\n":
            return "True"
        else:
            print "*** ERROR *** Cisco 4642HD Remote Control NOT found!"
            sys.exit()


def main(argv):

    s = configure_serial_port()

    if arduino_is_an_ir_tx(s):

        for command in args:
            s.write(command + "\n")
            time.sleep(4)
            response = s.readline().rstrip()

        if response != command:
            print "*** ERROR *** '" + command + "' !=  '" + response + "'"
        else:
            print "'" + command + "' sent"
        sys.exit()

if __name__ == "__main__":
    __version__ = "0.0.2"
    usage = "usage: %prog [options] <command>"
    parser = OptionParser(usage, version="%prog " + __version__)

    (options, args) = parser.parse_args()

    if len(args) == 0:
        parser.error("incorrect number of arguments")
        if options.verbose:
            print("reading %s..." % options.filename)

    if arduino_uno_connected():
        main(args)
