# Installation
This project was built around the SparkFun Pro Micro, this board isn't supported by the Arduino IDE by default but you can add it by reading the [Installing the Arduino Addon](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/installing-windows#windows_boardaddon) page on Sparkfun's website.

Once that is installed, you will also need to install some supporting libraries using the Arduino Libraries Manager;

 - "MCP23017" by Bertrand Lemalse version 2.0.0
 - "Adafruit TLC5947" by Adafruit version 1.0.6

These version numbers may change. Your mileage may vary but this code was developed using those specific versions.

# Configuration
All configuration is done via editing [config.h](https://github.com/jasongaunt/Sakey-Button-Box/blob/main/config.h) - here is the button layout;

![Button layout](/Button-Layout.png?raw=true)
