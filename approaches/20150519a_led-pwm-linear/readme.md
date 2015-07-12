<!-- vim: set ft=pandoc tw=80 spell: -->

# PWM Control, Linear Transition

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150519u200309`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This approach discusses using:

1.  LED control with PWM
2.  Linear transition between levels

## Overview

The idea is to use PIC PWM module to control the LED's brightness.

The output levels are directly mapped to the duty cycle. For 8b duty cycle
resolution, this gives 256 brightness levels, ranging from 0 (LED off), to 1
(full brightness).

Transition is done by changing the duty cycle in regular intervals. This is done
by using the separate timer, which on overflow updates the duty cycle. The
transition speed is determined by the update interval, that is, by the timer
initial value.

## Test Project

The test project is based on PIC12F615.

The LED PWM duty cycle change function is a triangle wave, with period of 4
seconds. PWM is obtained by using PWM module, connected to TMR2.

Duty cycle update is obtained through using TMR0, and interrupts.

PWM duty cycle resolution is 8 bits.

### Support

`20150704a_timer-calculator` support tool is used to calculate initial value for
TMR0. Parameter values are:

|Parameter         |Value           |
|------------------|----------------|
|clock             |4               |
|interval          |15.625          |
|timer width       |8               |
|init delay        |2               |

Interval is calculated like this: 2 (seconds, half period), divided by 256
(steps, PWM resolution).

The following resulting values are used:

|Result            |Value           |
|------------------|----------------|
|prescaler         |32              |
|TMR0 init val.    |14              |

### Hardware Setup

GP2 pin is used as PWM output. Other pins are unused (input).

