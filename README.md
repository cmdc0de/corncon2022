# corncon2022

Source code, build and assembly instructions

# Assembly of badge:

## Componets in your bag:
* 1 PCB board with all surface mounted components installed
* 7 LEDS <img src="images/led.png" alt="leds" width="50"/>
* 6 Buttons <img src="images/buttons.png" alt="button" width="50"/>
* IC Socket <img src="images/ic-socket.png" alt="socket" width="50"/>
* IC - shift register <img src="images/ic-shift-register.png" alt="shift register" width="50"/>
* 2 female headers <img src="images/10-pin-female.png" alt="header" width="50"/> <img src="images/4-pin-female.png" alt="header" width="50"/>
* 1 male header
* 1 LCD screen <img src="images/lcd.png" alt="lcd" width="50"/>
* 1 18650 Battery Holder <img src="images/battery-holder.png" alt="lcd" width="50"/>
* 1 18650 Battery
* 1 18650 Battery USB charger

## Assembly:
### LEDS
* place each led into the 7 LED locations on the top of the board ensuring the long leg of the LED through the whole labeled 'long'
* bend each of the 2 legs of the LED out such that if you turn the badge over they do not fall out.
* Solder each led of the 7 LEDs.

<img src="images/IMG_4736.jpg" alt="leds" width="200"/>

### Buttons
* place buttons in the 6 button wholes
* solder one leg of each button from the top of the board.
* Flip over board and solder both leds of each button

### IC Socket
* place IC Socket in the holes on the top of the pcb board with the notch towards the word notch
* Hold the IC socket in place and flip over the board, bend out legs in the 4 cornerns.
* Release your hold (socket should stay in place), solder all pins.

<img src="images/IMG_4735.jpg" alt="leds" width="200"/>

### Female headers
* Place the long and short female headers into the top of the board, holding the headers flip over the board and set down on solder table
* solder 1 lead on both the long and short header
* pick up the board and ensre the headers are straight, if not heat up the solder join and move the header straight
* solder the remaining header pins.

<img src="images/header-placement.jpg" alt="leds" width="200"/>

### Male header
* Place the 4 pin male header into the 4 pin female header.
* place the lcd pins into the longer female header and have the male header go through the 4 holes on the other side of the lcd
* solder the 4 pins to the lcd.

<img src="images/lcd.png" alt="lcd" width="200"/>


### Battery holder
* with the pcb board faing down: take the negative side of the battery holder (black wire) and place in the top left battery hole.
* Solder in place
* Take the postivie side of the battery holder (red wire) and place in the bottom right battery hole.
* Solder in place
* Turn the battery holder 180 degress, peal off tap covery from double sided tape.
* Press battery holder in place 

<img src="images/battery-holder.png" alt="battery holder" width="200"/>
<img src="images/battery-placement.jpg" alt="battery placement" width="400"/>

# Usage:
* Note: some of the power switches (just under the led below the LCD screen) does not work, meaning when you turn off the badge it won't turn off, you'll need to just pull the battery.  This is due to an issue with the manufacturing of the switches (but I ran out of time to get new ones).
* If after assembly a button does not work or the lcd screen is just white, ensure all header pins have solder, if that doesn't work.  reheat the esp32 microcontroller with a soldering iron along each pad.  Sometimes in transit the solder cracks between the esp32 and the pad.

## Main Menu:

<img src="images/IMG_4747.jpg" alt="battery holder" width="200" style="transform:rotate(90deg);"/>

* select any mode by pressing the fire button
* move back out of any menu by pressing the esc / jump button - except the "Test Badge" menu. where you must use esc/jump and right button to go back to the main menu

* Setting Menu - here you can set your badge name, register your badge (after you connected to wifi), set timezone, set the time until the badge goes to sleep.
<img src="images/IMG_4748.jpg" alt="battery holder" width="200" style="transform:rotate(90deg);"/>

* Screen Saver - ... its a screen saver :)
* Test Badge - this ensures all lights and buttons are working
* 3D Cube - check it out 
* Invaders - have fun!  for every color badge you pair you you gain 10%
<img src="images/IMG_4749.jpg" alt="invder" width="200" style="transform:rotate(90deg);"/>

* High Scores - see the high scores for invaders
* WiFi - allows you to connect to a wifi, it will first scan, then select a wifi and enter a password if need be.
<img src="images/IMG_4750.jpg" alt="wifi" width="200" style="transform:rotate(90deg);"/>
<img src="images/IMG_4751.jpg" alt="wifi" width="200" style="transform:rotate(90deg);"/>

* Connect Details - shows WiFi connection details
* Initiate Pair - If you want to pair with another badge and gain bonus points in Invaders click then, then have the badge your pairing with select pair and enter the pairing code. - Both badges must be connected to WiFi
* Pair - see above
* Sleep - put the badge to sleep to conserve battery.

# Build:
* TODO
