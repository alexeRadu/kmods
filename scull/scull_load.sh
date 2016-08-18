#!/bin/bash

module="scull"
device="scull"

sudo insmod scull.ko || exit 1

sudo rm -f /dev/${device}[0-3]

major=$(cat /proc/devices | grep ${module} | cut -d' ' -f1)
echo $major

sudo mknod /dev/${device}0 c $major 0
sudo mknod /dev/${device}1 c $major 1
sudo mknod /dev/${device}2 c $major 2
sudo mknod /dev/${device}3 c $major 3

sudo chown radu /dev/${device}0
sudo chown radu /dev/${device}1
sudo chown radu /dev/${device}2
sudo chown radu /dev/${device}3
