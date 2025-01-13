# autodesk-nano-card
Autodesk Fusion schematic and PCB design files, and sample programs to run on it
----------------------------------------------

These programs have been written to demonstrate an Arduino Nano-alike circuit on a PCB designed using Autodesk Fusion. The programs can easily be adapted to run on an Ardunio Uno, Nano, or any Atmega328-based system, with the appropriate hardware connected.<br>

The programs include:
- Snake - a basic implementation of the game Snake, using the SSD1306 OLED display (GPL v3)
- light-sensor - reads the voltage over the phototransostor as an analog value, and prints it on the OLED display
- rgb-led-demo - cycles through Red, Green and Blue colours on the built-in RGB LED. Holding the push button will change the pattern
- rgb-led-menu - allows the user to select specific values for the (PWM-controlled) brightness of the Red, Green and Blue of the RGB LED
