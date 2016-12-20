#!/bin/bash
#
# File Name: change_channel.sh
#
#  Copyright 2015-2016 Don Haig (time4tux at gmail dot com)
#  MIT Licence (See LICENSE.txt for details)
#
# Creation Date: 21-11-2015
LAST_MODIFIED="Tue 20 Dec 2016 04:42:44 PM EST -0500 "
VERSION=0.1	
#
# Purpose: A script for MythTV to call to change the channel on 
# 4642HD using an arduino base IR Tx
# 

LOG_FILE=/home/mythtv/change_channel.log

date=$(date +%Y-%m-%d-%H:%M:%S)
me=$(whoami)
if $(/home/mythtv/talk2irtx.py "CHANNEL $1" >> $LOG_FILE 2>&1); then
   # echo $result >> $LOG_FILE
   echo "$date 4642HD changed to channel'$1' $me" >> $LOG_FILE
   if $(/home/mythtv/pvr2_record.py $1 >> $LOG_FILE 2>&1 &); then
       echo "$date PVR2 record started" >> $LOG_FILE
   fi
fi
