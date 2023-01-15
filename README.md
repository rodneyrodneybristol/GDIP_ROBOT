# GDIP_ROBOT
<b>How to assemble and run</b>
<br>
To install:
<br>
```git clone https://github.com/rodneyrodneybristol/GDIP_ROBOT```
<br>
or download the .zip repository directly
<br>
Install platform IO, to open the firmware files for the motherboard and ohm meter board. 
<br>
Firmware may take a little while to load and build initially if you do not already have the Pi Pico SDK and Arduino frameworks installed. 
<br>
Firmware can be loaded to the Pi Pico directly and the Atmega328p can be programmed using an AVR programmer or Arduino in ISP programmer mode.
<br>
to 3D print the conveoyr belt, simply load the provided STLs in to any chopper software. Best to print them vertically to avoid excessive infil.
<br>
2 x 5V supplies, with at least 2A each are required  to run this system, one which handles the mobo/servos/ohm meter etc and another for powering the conveoyrs NEMA17 stepper motor. 
<br>
Calibration offset for the Robot can be adjusted in the ``` startAndCalibrateArm();``` function within the mobo/mainboard main.cpp file. These will need to be set once when loading the system, in case any servos have serious offsets. This allows for more precise movements of the arm.
<br>
