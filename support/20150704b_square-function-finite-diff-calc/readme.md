<!-- vim: set ft=pandoc tw=80 spell: -->

# Square Function Finite Differences Calculator

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150704u172454`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This is the support tool for the `20150630u175146` project. It helps in finding
the optimal parameter values for square function transition.

## Use

It works like this:

1.  Enter the required number of steps for the "speeding-up" section of
    transition. If total transition has 255 steps, 127.5 is entered.
2.  Enter duration for this section of transition. If total transition takes 2
    seconds, enter 1000 (milliseconds).
3.  The output is calculated for the series of update intervals. This series is
    specified with
    1.  Smallest update interval. For the example, let's say that 3000
        (milliseconds) is a good value.
    2.  Series increment step. In example it is 1 ms.
4.  Enter finite difference variables width, in bits. 16b is the example value.
5.  Enter the result value width, in bits, 8b is used (for 256-entries lookup
    table).
6.  Go through the table, and try to find if there are green fields under
    "Configuration ok?"
    1.  If there is no such field, options are:
        1.  Change input for the update interval. Maybe some timer value outside
            the current sweep will yield an usable interval.
        2.  Increase finite difference variables width.
        3.  Change the transition parameters (duration, steps).
7.  For such fields, find the one with the smallest `Δy[last]` (the result will
    most closely match the intended output), and with the smallest `Δt [ms]`
    (the transition will end more closely to the intended time).
8.  Snatch the following values, and use them in the program, or other support
    tools:
    1.  `Δ2y mul round`
    2.  `Dx [us]`
    3.  `iterations (round)`

