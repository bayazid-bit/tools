#!/bin/bash

inter="$1"

showhelp() {
cat <<EOF
Change MAC address every 5 seconds.
Simple tool by MD. BAYAZID (SSC25) :)

Usage: sudo ./mctimer.sh <interface name>

Example: sudo ./mctimer.sh wlan0
EOF
}


if [[ $(whoami) != "root" ]]; then
    echo "Error: Please run it as root!"
    echo "Try: sudo $0 <interface>"
    exit 1
fi


if [[ -z "$inter" ]]; then 
    showhelp
    exit 1 
fi 

change_mac() {
    ip link set "$inter" down
    macchanger -r "$inter"
    ip link set "$inter" up
}


while true; do 
    change_mac 
    sleep 60
done
