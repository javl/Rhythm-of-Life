#! /usr/bin/env python
import sys
import serial
import commands


def main():
    try:
        # Get current Arduino address as this can change; this might work differently on Linux
        # status, address = commands.getstatusoutput('ls /dev | grep tty.usbmodem')
        # if address == "":
        status, address = commands.getstatusoutput('ls /dev | grep ttyACM')
        if address == "":
            exit("Sorry, can't find Arduino...")
        print "Found Arduino at addres:", address
        address = "ttyACM1"
        print "Please use 'x' to exit, instead of ^C"
        ser = serial.Serial("/dev/" + address, 115200)
        # ser = serial.Serial("/dev/ttyACM0", 115200)
        # Set the pullTimer to 100ms
        ser.write('p<100>')

        while(1):
            myInput = raw_input("Enter a number (0-9) or r<number> / p<number> to set release/pull timers, x to exit: ")
            ser.write(myInput)
            if myInput == 'x':
                ser.close()
                sys.exit(0)

    except KeyboardInterrupt:
        # ser.close()
        sys.exit(0)
    except Exception:
        # ser.close()
        sys.exit(0)


if __name__ == "__main__":
    main()
