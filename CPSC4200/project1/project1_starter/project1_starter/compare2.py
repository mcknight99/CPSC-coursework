#!/usr/bin/env python3

# Read the prefix so we know where the collision blob begins.
with open("prefix", "rb") as f:
    prefix = f.read()

# Read both generated collision files.
with open("col1", "rb") as f:
    col1 = f.read()

with open("col2", "rb") as f:
    col2 = f.read()

# The collision data starts immediately after the prefix.
blob1 = col1[len(prefix):]
blob2 = col2[len(prefix):]

print(f"Prefix length: {len(prefix)}")
print(f"Blob length:   {len(blob1)}")

# Find a differing byte where parity is different.
for N, (a, b) in enumerate(zip(blob1, blob2)):
    if a != b and (a % 2) != (b % 2):
        print(f"\nUse N = {N}")
        print(f"blob1[{N}] = {a} (0x{a:02x})")
        print(f"blob2[{N}] = {b} (0x{b:02x})")
        print(f"Parity: {a % 2} vs {b % 2}")
        break
else:
    print("No suitable byte found.")