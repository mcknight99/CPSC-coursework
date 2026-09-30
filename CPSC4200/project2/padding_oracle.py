#!/usr/bin/python3

# Run me like this:
# $ python3 padding_oracle.py "http://cpsc4200.mpese.com/username/paddingoracle/verify" "5a7793d3..."
# or select "Padding Oracle" from the VS Code debugger

import json
import sys
import time
from typing import Union, Dict, List

import requests

# Create one session for each oracle request to share. This allows the
# underlying connection to be re-used, which speeds up subsequent requests!
s = requests.session()


def oracle(url: str, messages: List[bytes]) -> List[Dict[str, str]]:
    while True:
        try:
            r = s.post(url, data={"message": [m.hex() for m in messages]})
            r.raise_for_status()
            return r.json()
        # Under heavy server load, your request might time out. If this happens,
        # the function will automatically retry in 10 seconds for you.
        except requests.exceptions.RequestException as e:
            sys.stderr.write(str(e))
            sys.stderr.write("\nRetrying in 10 seconds...\n")
            time.sleep(10)
            continue
        except json.JSONDecodeError as e:
            sys.stderr.write("It's possible that the oracle server is overloaded right now, or that provided URL is wrong.\n")
            sys.stderr.write("If this keeps happening, check the URL. Perhaps your uniqname is not set.\n")
            sys.stderr.write("Retrying in 10 seconds...\n\n")
            time.sleep(10)
            continue

BLOCK_SIZE = 16


def padding_is_valid(status: str) -> bool:
    """
    The oracle exposes different errors depending on how far decryption gets:

    invalid_padding means our guess was wrong
    invalid_mac means padding was valid, but the decrypted message failed the HMAC check
    valid also means the padding was valid

    Therefore both invalid_mac and valid indicate successful padding
    """
    return status != "invalid_padding"


def decrypt_block(url: str, previous_block: bytes, ciphertext_block: bytes) -> bytes:
    """
    Recover the plaintext for one CBC ciphertext block

    CBC decryption is:
        P = D(C) XOR IV

    For a normal ciphertext block, the previous ciphertext block acts as the IV

    We manipulate the previous block until the decrypted plaintext ends in valid PKCS#7 padding
    """

    # intermediate is D(C) before XORing with the previous block
    # Once we know:
    #     intermediate[i] = D(C)[i]
    # the original plaintext is:
    #     plaintext[i] = intermediate[i] XOR previous_block[i]
    intermediate = bytearray(BLOCK_SIZE)

    # recover bytes from right to left
    for position in range(BLOCK_SIZE - 1, -1, -1):

        # PKCS#7 padding value we want to create
        padding_value = BLOCK_SIZE - position

        # build a modified copy of the previous ciphertext block
        base = bytearray(previous_block)

        # force every byte that we have already solved to equal the desired padding value
        for i in range(position + 1, BLOCK_SIZE):
            base[i] = intermediate[i] ^ padding_value

        # try every possible value for the current byte
        guesses = []

        for guess in range(256):
            modified = bytearray(base)
            modified[position] = guess

            # the oracle only needs the modified previous block followed by the target ciphertext block
            guesses.append(bytes(modified) + ciphertext_block)

        # send all 256 guesses in one request (so we don't crash the server with 256 requests :) )
        results = oracle(url, guesses)

        candidates = []

        for guess, result in enumerate(results):
            if padding_is_valid(result["status"]):
                candidates.append(guess)

        if not candidates:
            # print as much as possible to see what was recovered
            print(f"Recovered: {intermediate!r}\n")
            raise RuntimeError(
                f"Could not find valid padding at byte {position}"
            )

        # normally there is exactly one candidate, but at the final byte, there can occasionally be
        # multiple candidates because the plaintext may already have valid padding.
        # resolve ambiguity with an additional query by changing a byte immediately before the padding byte
        # if len(candidates) > 1 and position == BLOCK_SIZE - 1:
        #     print("ope")
        #     real_candidates = []

        #     for guess in candidates:
        #         print(f"    testing candidate {guess:02x}...", file=sys.stderr)
        #         test = bytearray(base)

        #         # if our candidate really creates valid padding, disturbing this byte should preserve validity
        #         test[position - 1] ^= 1

        #         result = oracle(url, [bytes(test) + ciphertext_block])[0]

        #         if padding_is_valid(result["status"]):
        #             real_candidates.append(guess)

        #     if len(real_candidates) == 1:
        #         candidates = real_candidates
        #         print(f"    candidate {candidates[0]:02x} is the real one", file=sys.stderr)

        # select the candidate
        guess = candidates[0]

        # if the desired plaintext byte is:
        #     P[position] = intermediate[position] XOR original_previous
        # and we deliberately changed the previous byte to 'guess',
        # then:
        #     intermediate[position] = guess XOR padding_value
        intermediate[position] = guess ^ padding_value

        print(
            f"    recovered byte {position:2d}: "
            f"{intermediate[position]:02x}"
        )

    # convert intermediate bytes into actual plaintext bytes
    plaintext = bytes(
        intermediate[i] ^ previous_block[i]
        for i in range(BLOCK_SIZE)
    )

    return plaintext


def remove_pkcs7_padding(data: bytes) -> bytes:
    """
    Remove PKCS#7 padding from the recovered plaintext.
    """
    if not data:
        raise ValueError("Empty plaintext")

    padding_length = data[-1]

    if padding_length < 1 or padding_length > BLOCK_SIZE:
        # print as much as possible to see what was recovered
        print(f"Recovered: {data!r}", file=sys.stderr)
        raise ValueError("Invalid PKCS#7 padding")

    if data[-padding_length:] != bytes([padding_length]) * padding_length:
        # print as much as possible to see what was recovered
        print(f"Recovered: {data!r}", file=sys.stderr)
        raise ValueError("Invalid PKCS#7 padding")

    return data[:-padding_length]


def main():
    if len(sys.argv) != 3:
        print(
            f"usage: {sys.argv[0]} ORACLE_URL CIPHERTEXT_HEX",
            file=sys.stderr,
        )
        sys.exit(-1)

    oracle_url = sys.argv[1]
    ciphertext = bytes.fromhex(sys.argv[2])

    # AES has a 16-byte block size
    if len(ciphertext) % BLOCK_SIZE != 0:
        raise ValueError("Ciphertext length is not a multiple of 16")

    # check that the supplied ciphertext is initially valid
    initial_status = oracle(oracle_url, [ciphertext])[0]["status"]

    if initial_status != "valid":
        print(
            f"Warning: original ciphertext returned {initial_status}",
            file=sys.stderr,
        )

    # split the ciphertext into 16-byte blocks
    blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    # blocks[0] is the IV
    # blocks[1:] are the actual encrypted blocks
    recovered = bytearray()

    print(
        f"Ciphertext contains {len(blocks) - 1} encrypted blocks.",
        file=sys.stderr,
    )

    # decrypt every ciphertext block independently
    # for C_i, we manipulate C_(i-1)
    for block_number in range(1, len(blocks)):
        print(
            f"\nDecrypting block {block_number}/{len(blocks) - 1}...",
            file=sys.stderr,
        )

        plaintext_block = decrypt_block(
            oracle_url,
            blocks[block_number - 1],
            blocks[block_number],
        )

        recovered.extend(plaintext_block)

        print(
            f"    plaintext: {plaintext_block!r}",
            file=sys.stderr,
        )

    # remove the PKCS#7 padding added by AES-CBC
    plaintext = remove_pkcs7_padding(bytes(recovered))

    print("\nRecovered plaintext:", file=sys.stderr)
    print(repr(plaintext), file=sys.stderr)

    # The server encrypted:
    #     message || HMAC-SHA256(message)
    if len(plaintext) >= 32:
        message = plaintext[:-32]
        mac = plaintext[-32:]

        print("\nMessage:", file=sys.stderr)
        print(repr(message), file=sys.stderr)

        print("\nHMAC:", file=sys.stderr)
        print(mac.hex(), file=sys.stderr)

        print("\nFLAG / MESSAGE:")
        try:
            print(message.decode())
        except UnicodeDecodeError:
            print(message)

    else:
        print("\nRecovered plaintext:")
        try:
            print(plaintext.decode())
        except UnicodeDecodeError:
            print(plaintext)


if __name__ == "__main__":
    main()