# BatteryMeterSimulator

## General Overview
Program run by a microcontroller in a digital battery meter. The hardware has the following relevant features.

- A voltage sensor whose value can be accessed via a memory mapped port. In C this is presented as a global variable. Another port indicates whether the voltage should be displayed in Volts or Percentage of total battery capacity.

- A digital display with a port control port; setting certain global variable will change the display to show information to a user of the battery meter.

- A simulator program with Makefile to test your code

## Usage
> make batt_main

> ./batt_main 7884 V      (volts option)


OR 


> ./batt_main 7884 P      (percentage option)

## Contributing
Pull requests are welcome. :)

## Ownership
Poojan Patel (poojanp00@gmail.com)

Fall 2019

