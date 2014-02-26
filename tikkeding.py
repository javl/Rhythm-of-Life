#! /usr/bin/env python
import sys, serial, commands

def main():
	try:
		# Get current Arduino address as this can change (this might work differently on Linux)
		status, address = commands.getstatusoutput('ls /dev | grep tty.usbmodem')
		if address == "": exit("No Arduino found...")

		ser = serial.Serial("/dev/"+address, 115200)

		# Set both the pullTimer 40ms
		ser.write('p<40>')

		while(1):
			myInput = raw_input("Enter a number (0-9) or r<number> / p<number>, x to exit: ")
			ser.write(myInput)
			if myInput == 'x':
				ser.close()
				sys.exit(0)
	except KeyboardInterrupt:
		ser.close()
	except Exception:
		ser.close()
		sys.exit(0)

if __name__ == "__main__":
	main()