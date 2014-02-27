#Tikkeding#

###To do###
* Check if the path to the Arduino can be found on the production system.
`tikkeding.py` searches for `tty.usbmodem*` in `/dev`. This works on OSX, but might be different on another OS.

###Arduino Setup###
Connect the Arduino over USB and upload `tikkeding.ino` from the Arduino IDE.

###Control Box Setup###
Plug in the power cord and set set the switch on the back (below the power cord) to the on position. When the green LED on the front stops blinking, connect the box to the computer using a USB cable.

###Computer Setup###
After making sure the LED on the control box is a solid green, run the script with `python tikkeding.py` – or alternatively, make the file executable with `chmod +x tikkeding.py` and run it with `./tikkeding.py`. The orange LED on the control box will light up when a serial connection has succesfully been established.

###Controlling the Solenoids###
The solenoids can be triggered by sending their index number over the serial connection. They will be activated for 40ms by default – this value can be changed from the computer, see the commands below. Active solenoids will ignore any new command during this period.

| Command | Description | Note | Example |
| :------------- | :----------- | :----------- | :----------- |
| 0 ... 6    | Solenoids are triggered by sending their index number. | You can send multiple numbers at the same time. |  `ser.write('5')` `ser.write('0246')` |
| p\<number\>     | Set the pull time for all solenoids, in milliseconds. Without sending this command, a default value of 40ms will be used. | The < and > characters need to be included in the command. | `ser.write('p<40>')` |

###Testing the Hardware###
The button on the front of the control box can be used to test the solenoids and enable certain special functions:
* At startup, the first solenoid is 'selected' and can be triggered by pressing the button once.
* Pressing the button twice will move the selection one over to the next solenoid or – if the last solenoid is already active – will select all of the solenoids at the same time.
* Pressing the button three times will activate a special function, depending on the currently selected solenoid (see below). Pressing the button while a special function is running will stop it.
 
| Selected solenoid | Special function |
| :-------------| :-------------|
| 1| Enable random mode|
| 2| Enable Knight rider mode|
