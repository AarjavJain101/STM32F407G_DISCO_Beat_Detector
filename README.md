# Minimum Viable Product: Using LED Strip as Medium for 2023 Beat Detector Algorithm
The goal of this project is to use the 2023 Beat Detector algorithm on an LED strip (WS2812B) on any development board using the HAL API. The board of choice in my case is the STM32F407G-DISC Rev E01.

## Usage
### Important aterials
* An STM32F407G-DISC Rev E01. Other revisions and just the STM32F407VGT6 chip should work, however, the program is not tested on those.
* An I2S output microphone. I am using the SPH0645LM4H adafruit mic.
* USB A to USB Mini A
* WS2812B LED light strip
* Adjustable Power Supply (higher amperage at 5V is better).

### Set-Up
* First clone the repository.
* Connect the USB cable from device to board and flash the program onto the board.
* Unplug the USB cable from the board now.
* Then connect the 5V and GND pins on the board to the red and white lines respectively on the WS2812B LED strip (this assumes you are using BTF lighting's model. If not refer to the LED strip data sheet for the correct power lines).
* Connect pin `PE9` to green line on the LED strip (this is PWM out pin).
* Now to connect the SPH0645LM4H mic make the following connections
    * 
* Now, power the board either with the adjustable power supply or with the USB cable again and the LED strip is now sound reactive!
