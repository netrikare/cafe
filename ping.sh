#!/usr/bin/env bash
IP="192.168.0.2"

#Reboot
#ping -c 1 -v -p 616162 $IP

#Clear all firewall rules: iptables -F
ping -c 1 -v -p 626162 $IP

#Allow IP
#ping -c 1 -v -p 636162 $IP

#Reset firewall
#ping -c 1 -v -p 646162 $IP
