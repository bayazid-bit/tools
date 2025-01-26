#!/bin/bash

if [ "$EUID" -ne 0 ] ; then 
    echo " YOU NEED TO BE ROOT TO RUN THIS SCRIPT " 
    exit 1 
fi 

i=1 
interface="wlp2s0"
while [ $i -le 5 ] ; do 
    ifconfig $interface down
    macchanger -r $interface 
    ifconfig $interface up 
    i=$(( i+1 )) 
done 
