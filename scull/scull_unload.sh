#!/bin/bash

device="scull"

sudo rmmod scull.ko

sudo rm -f /dev/${device}[0-3]
