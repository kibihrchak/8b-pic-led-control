#!/usr/bin/env python3

import time
import math

_FILENAME = 'cie-lb-lookup'
_ORIGINAL_BIT_WIDTH = 16
_TARGET_BIT_WIDTH = 10

lookup_table = list()

with open(_FILENAME, 'r') as f:
    for raw_line in f:
        for column in raw_line.strip().split():
            lookup_table.append(column)

print("{")

i = 0

bit_width_diff = _ORIGINAL_BIT_WIDTH - _TARGET_BIT_WIDTH

while i < len(lookup_table):
    line = str()

    for j in range(8):
        value = int(math.trunc(float(abs(int(
            lookup_table[i]) - 65535)) / math.pow(2, bit_width_diff)))
        line += "{0:>8d},".format(value)
        i += 1

    print(line)

print("}")
