#!/bin/sh

echo 'Starting PWM'

#EHRPWM1A | P9_14 | pwmchip 4, channel 0

config-pin -q P9_14
config-pin P9_14 pwm
cd /sys/class/pwm/pwm-4:0 #entering chip 4 directory for editing settings


echo 100000000 > period
echo 50000000 > duty_cycle
echo 1 > enable




