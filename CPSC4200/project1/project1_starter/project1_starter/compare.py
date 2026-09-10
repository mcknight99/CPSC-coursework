#!/usr/bin/env python3

# read both files as raw bytes
with open("col1", "rb") as f:
    col1 = f.read()

with open("col2", "rb") as f:
    col2 = f.read()

if len(col1) != len(col2):
    raise ValueError("The collision files have different lengths")

# look for a differing byte where one value is even and the other is odd
for N, (a, b) in enumerate(zip(col1, col2)):
    if a != b and (a % 2) != (b % 2):
        print(f"N = {N}")
        print(f"col1 byte = {a} (0x{a:02x}) -> {a % 2}")
        print(f"col2 byte = {b} (0x{b:02x}) -> {b % 2}")
        break
else:
    print("No differing byte with opposite parity was found.")

