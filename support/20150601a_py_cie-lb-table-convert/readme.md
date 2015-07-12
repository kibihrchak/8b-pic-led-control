<!-- vim: set ft=pandoc tw=80 spell: -->

# CIE Lightness to Brightness Table Conversion

|Field              |Value                  |
|-------------------|-----------------------|
|Author             |Marko Oklobdžija       |
|Timestamp (UTC)    |`20150601u215600`      |

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

This is a tool used to convert the lookup table snatched from this page
<https://ledshield.wordpress.com/2012/11/13/led-brightness-to-your-eye-gamma-correction-no/>
to the requred (lower) bit width.

Input table is in the `cie-lb-lookup` file.

## Scipt Parameters

Parameter               Description
----------------------- ------------------------------------------------
`_TARGET_BIT_WIDTH`     Target output value width, in bits

