<!-- vim: set ft=pandoc tw=80 spell: -->

# Timer Calculator

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150704u163843`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This is a little spreadsheet tool used to calculate the initial value for a
timer using the increment counter, so that the given time interval expires on
counter overflow.

## Input Parameters

Input parameter                 Description
------------------------------- ---------------------------------------
`clock [MHz]`                   MCU clock
`interval [ms]`                 Required time interval
`timer width [b]`               Timer width, in bits
`timer init delay [cycles]`     Timer delay after value set, in cycles

This tool is written for Microchip PIC10/12/16 MCUs, where the instruction
frequency is 1/4 of the clock frequency, and timer increments on every
instruction cycle.

