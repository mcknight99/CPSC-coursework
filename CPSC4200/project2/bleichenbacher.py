#!/usr/bin/python3

# Run me like this:
# $ python3 bleichenbacher.py "coach+spupke+100.00"
# or select "Bleichenbacher" from the VS Code debugger

from roots import *

import hashlib
import sys


# 2048-bit modulus from the "coach" public key
N = int(
    "b5742572bbe0384b1bc7cf45c7499ee47e20e55c0b02e5e5e7af38ba00ada074aa85824a2d12f7ae3b1fdbce26c31af9a66e2b010efac97f9a36c589160742d88a31327abf9181eb3038270d572c0d476c0bc11eeb575650842068d31d7f1e3cf91e8c06c92f4b2bd39a6c012be1511af97fec341c9432c22ea4ce3797eb880f0a807b1e5dda921c2cab7d5133115efd388c833bbc6771e0d7605e4f3f938bd0bd3d186ffd60e630aecd53111d7ab93ca00cf2a6b54ccb2d24759a9447b49cd2a5f206fe7e5690f065477c19123746be18f51311148316e1bf52fb503a10648db087c62fcaa3358d9ef9bd1271dc6eb107901f09d8cc0268b55ac89a78410bd633",
    16
)

E = 3

# ASN.1 DigestInfo prefix for SHA-256:
# 30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20
SHA256_ASN1 = bytes.fromhex(
    "3031300d060960864801650304020105000420"
)


def main():
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} MESSAGE", file=sys.stderr)
        sys.exit(-1)

    message = sys.argv[1].encode()

    digest = hashlib.sha256(message).digest()

    # prefix that the server actually checks
    expected_prefix = (
        b"\x00\x01\xff\x00"
        + SHA256_ASN1
        + digest
    )

    target_bytes = expected_prefix + bytes(256 - len(expected_prefix))
    target = bytes_to_integer(target_bytes)

    root, exact = integer_nthroot(target, E)

    forged_signature = None

    # try floor(cuberoot(target)) and the next integer. the exact root is not expected to be an integer
    for candidate in (root, root + 1):
        cube = candidate ** E

        cube_bytes = integer_to_bytes(cube, 256)

        # make sure the resulting RSA block starts with the structure the server expects
        if cube_bytes.startswith(expected_prefix):
            print(f"Candidate {candidate} succeeded: {cube_bytes.hex()}")
            forged_signature = candidate
            break
        else: 
            print(f"Candidate {candidate} failed: {cube_bytes.hex()}")

    if forged_signature is None:
        raise ValueError("Could not construct a valid forged signature")

    # the bank expects the signature as base64
    signature_bytes = integer_to_bytes(forged_signature, 256)
    print("Forged signature (base64):")
    print(bytes_to_base64(signature_bytes))


if __name__ == '__main__':
    main()