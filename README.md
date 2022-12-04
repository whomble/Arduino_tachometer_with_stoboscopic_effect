# Arduino_tachometer_with_stoboscopic_effect

<img src="img/PXL_20221204_132251795.MP.jpg" width="300"><img src="img/PXL_20221204_132245093.MP.jpg"  width="300"><img src="img/PXL_20221204_132311039.MP.jpg"  width="300">

The purpose of this tool is to measure any rotation speed with the same accuracy as a commercial product,
it can also blink at afrequency defined by the user to match the roatation of anything up to 1Khz.

It can be powered with usb (5V) or with a 12V power supply, when it's connected to 12V the tachometer can output 12V (20A max) at the blinking frequency. 
Since my workshop has a 12V power line to feed my LED lights and tools this allow me to blink all my lights simultaneously to analyze the rotation of any tool.

The left gift show my spindle's CNC at 12 000rpm with the matching frequency. The other gift show the same setup with a slightly different blinking frequency.

<img src="gif/strobosc.gif" width="450"><img src="gif/strob.gif"  width="450">


# Part list
| Part | Quantity | Cost |
|- | - | - |
| Arduino Nano | 1 | 5€ |
| IR barrirer sensor | 1 | 2€ |
| White LED | 1 | 0€ |
| Male + female jack connector| 1 | 3€ |
| 0.96" OLED display | 1 | 3€ |
| IRFZ44N Mosfet | 1 | 3€ |
| 3 pos sliding switch | 1 | 2€ |
| Rotary encoder | 1 | 2€ |
| TC4428 Mosfet driver | 1 | 5€ |


# 3D Printing


# Wirring

The wirring isn't verry complicated, the only tricky part is the mosfet wirring.
Since its a N-Channel Mosfet the drain (middle part) is connected to the output and the source to the power suply's ground.
The gate is connected to a non inverting output from the Mosfet driver, and the driver itself is powered by the 12V and the signal is send by a PWM pin from the arduino.

All the others connextion can be found on the schematic below and in the code!


<img src="img/Capture2.JPG" width="900">

# Code
