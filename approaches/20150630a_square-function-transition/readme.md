<!-- vim: set ft=pandoc tw=80 spell: -->

# PWM Control, Square Transition, Brightness Lookup Table

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150630u175146`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This approach is based on the `20150601u140000` one. It discusses:

1.  Obtaining non-linear transition between levels (transition smoothing)

## Overview

This approach builds up on the `20150601u140000` one. The aim is to make level
transition smoother. The transition algorithm must satisfy the following
requirements:

1.  Transition time must not be fixed, but specified for each transition
2.  Smoothing must be done the same way independent of the level distance
3.  Operation must not be expensive - only addition, subtraction on integers is
    allowed

### Lookup Table Approach

The easiest way to get smooth transition, is to integrate smoothing in the
lookup table, like here -
<http://osx-launchpad.blogspot.com/2010/11/breathing-led-effect-with-launchpad.html>.
But, this approach does not satisfy the second requirement.

### Function Approach

Another idea is to use some math function, calculated in regular time intervals,
and used to calculate the lookup table index. That approach is used here -
<http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/>.

The problem with it is that it does not satisfy the third requirement.

### Finite Differences Approach

This approach goes in the following direction - Is there some function which
calculation can be translated in the series of "cheap" operations (addition,
subtraction), preferably building upon the previous result, yet it produces the
pleasing transition?

The answer lies in finite differences
(<https://en.wikipedia.org/wiki/Newton_polynomial>). If the function can be
represented using finite differences, it is easy to calculate its value for each
step, using just simple operations. In addition, if the finite differences are
not of the high order, and the last one is constant, this gives quite a simple
setup for calculating function value.

The promising candidate is a square function. Last non-zero finite difference is
of second order, and is constant. Example:

|x      |y      |Δy     |Δ2y    |Δ3y    |
|-------|-------|-------|-------|-------|
|0      |0      |1      |2      |0      |
|1      |1      |3      |2      |0      |
|2      |4      |5      |2      |0      |
|3      |9      |7      |2      |       |
|4      |16     |9      |       |       |
|5      |25     |       |       |       |

Now, square function smoothes the initial transition, but it speeds up with
time, and does not ease down. The solution for this issue is to use square
function for the "speeding-up" segment of transition, and use reverse square
function (`y = -x^2`) for the "slowing-down" segment. They would be of the same
nature, that is, with the same coefficients, and same finite differences, just
inversed. This means, when the half of the transition interval is reached, the
process just needs to be reversed.

So, for example, if the transition is done between values 0, and 2, in the time
interval of 2s, and there are 512 iterations, this yields:

|Parameter         |Value           |
|------------------|----------------|
|Update interval   |3.906ms         |
|Initial y         |0               |
|Δ2y               |0.000031        |

## Test Project

It is based on `20150601u140000`. Finite differences approach is implemented.

The LED PWM lookup table index change function has a period of 4 seconds. Each
half-period consists out of two parts. In the first part the output is a square
function of time, in the second part, inverse of the first part.

PWM duty cycle resolution is 10 bits.

### Finite Differences Approach

The issue here lies in the limited resolution of variables used on MCU, and that
they are all integers. This results in:

1.  Update interval cannot be precise, so the transition can end up before, or
    after the intended time
2.  Δ2y cannot be represented with the sufficient precision

The second problem is of significant importance. If Δ2y is smaller than the
required value, the transition will fall short of the target value. If it is
larger than the required value, overshoot will occur, and possibly, overflow.

The solution to this problem is twofold:

1.  Extend precision for variables - Even if 8 bits are required to address the
    entry in the lookup table (or to control PWM duty cycle directly) it is
    possible to use bigger variables, and perform fixed points arithmetic.
    Example - for 16b result value, upper 8 bits can be used for lookup table
    addressing, and lower 8b for added precision.
2.  Adjust update interval, to obtain second order difference which gives
    smaller error when rounded.

The second idea is that the duty cycle value update is not tied to the update
interval. It is possible to have multiple update intervals per one duty cycle
value update. By adjusting the update (calculation) interval, it is possible to
get the more usable values.

### Support

The above talk is translated to the support table `20150704u172454`. It is used
to find the optimal parameters.

### Hardware Setup

GP2 pin is used as PWM output. Other pins are unused (input).

