#! /usr/bin/env python
import serial, commands

# Get current Arduino address as this can change (this might work differently on Linux)
status, address = commands.getstatusoutput('find /dev -name "tty.usbmodem*" -maxdepth 1')
if(address == ""):
	print "No Arduino found. Breaking"
	exit();

ser = serial.Serial(address, 115200)

# Set both the pullTimer and releaseTimer to 100ms
ser.write('p<100>')
ser.write('r<100>')

while(1):
	myInput = raw_input("Enter a number in (0-9) or r<number> / p<number>: ")
	try:
		ser.write('%d' % int(myInput))
	except:
		ser.write(myInput)