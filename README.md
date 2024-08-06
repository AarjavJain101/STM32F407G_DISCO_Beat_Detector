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
* Next connect the LED strip to the board as follows:
    * 5V and GND pins on the board to the red and white lines respectively on the WS2812B LED strip (this assumes you are using BTF lighting's model. If not refer to the LED strip data sheet for the correct power lines).
    * Connect pin `PE9` to green line on the LED strip (this is PWM out pin).
* Now to connect the SPH0645LM4H mic make the following connections
    * **WS**: Connect `LRCL` on mic to `PA4` on the chip/board
    * **CLK**: Connect `BCLK` on mic to `PC10` on the chip/board
    * **DATA**: Connect `DOUT` on mic to `PC12` on the chip/board
    * **3V**: Connect `3V` on mic to `3V` on the chip/board
    * **GND**: Connect `GND` on mic to `GND` on the chip/board
* Now, connect the USB A to USB micro A to the board from your computer with the repository. Using the STM32CubeMX software run/flash the firmware onto the board
* Finally, play music and enjoy the beat detector :).
