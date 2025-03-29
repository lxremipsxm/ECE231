#!/bin/sh

echo 'Stopping PWM'

cd /sys/class/pwm/pwm-4:0

echo 0 > enable
