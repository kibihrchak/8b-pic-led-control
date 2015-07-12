<!-- vim: set ft=pandoc tw=80 spell: -->

# PWM Control, Linear Transition, Brightness Lookup Table

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150601u140000`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This approach is based on the `20150601u140000` one. It discusses:

1.  Using lookup table to make brightness control using duty cycle linear

## Overview

This approach builds up on the `20150519u200309` one. The aim is to linearize
the LED brightness, by providing the lookup table which would map duty cycle,
that is, current, that is, luminosity, to brightness.

### LED Current, vs. Brightness, vs. Luminosity

This is the thing - LED brightness should be linear with respect to the current
passed through it. If LED is a linear element, this would translate to the
linear relationship between voltage, and luminosity. But this is not the case,
LEDs being non-linear. This would represent a potential issue, because MCU GPIOs
are a voltage, not current sources.

Now, the fortunate thing about PWM is that the "on" period has a fixed voltage,
with LED current being constant in that region. By changing the duty cycle, the
average current would change with duty cycle, that is, current will be linearly
dependant on the duty cycle.

Next issue is with the perceived brightness. It is not linearly dependant on
brightness.

The easiest way to resolve this issue is to use a lookup table, which would
translate old duty cycle index (luminosity) to new duty cycle index
(brightness).

The lookup table is found here:
<https://ledshield.wordpress.com/2012/11/13/led-brightness-to-your-eye-gamma-correction-no/>.

## Test Project

This test project is built upon the one from `20150519u200309`. The PWM
resolution is expanded to 10b.

### Support

In addition to the old tool, `20150601u215600` one is used to convert the lookup
table found on the link mentioned above, to the format suitable for use in the
test project.

### Hardware Setup

GP2 pin is used as PWM output. Other pins are unused (input).

