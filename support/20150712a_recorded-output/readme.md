<!-- vim: set ft=pandoc tw=80 spell: -->

# Recorded Output

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150712u091743`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

The photos from the oscilloscope screen, representing output for LED control
approaches.

## Overview

Here are the outputs recorded from the MikroC projects:

1.  Linear PWM change (triangle wave)
2.  Linear PWM change, with lookup table
3.  Square PWM change
4.  Square PWM change, with lookup table

## Hardware

The MCU output (PWM signal) is passed through passive low-pass RC filter to get
the average duty cycle value. This signal is then picked by oscilloscope. So,
oscilloscope output represents the average current on LED.

