#!/usr/bin/python3

# Run me like this:
# $ python3 len_ext_attack.py "http://cpsc4200.mpese.com/uniqname/lengthextension/api?token=...."
# or select "Length Extension" from the VS Code debugger

import sys
from urllib.parse import quote
from pysha256 import sha256, padding

# water lawn:
# http://cpsc4200.mpese.com/spupke/lengthextension/api?token=499eb4b396f3c5b2b66a14fd0a22ac107605f737132ba7c2855291d68bebaa6c&command=SprinklersPowerOn
# reset clock:
# http://cpsc4200.mpese.com/spupke/lengthextension/api?token=15a80ed015539ecc9b854e161f503870021dfc089583ca6868992c5098259c48&command=ClockPowerOff&command=NoOp&command=ClockPowerOn
# invalid unlocksafes:
# http://cpsc4200.mpese.com/spupke/lengthextension/api?token=&command=UnlockSafes

class URL:
    def __init__(self, url: str):
        # prefix is the slice of the URL from "http://" to "token=", inclusive.
        self.prefix = url[:url.find('=') + 1]
        self.token = url[url.find('=') + 1:url.find('&')]
        # suffix starts at the first "command=" and goes to the end of the URL
        self.suffix = url[url.find('&') + 1:]

    def __str__(self) -> str:
        return f'{self.prefix}{self.token}&{self.suffix}'

    def __repr__(self) -> str:
        return f'{type(self).__name__}({str(self).__repr__()})'


def main():
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} URL_TO_EXTEND", file=sys.stderr)
        sys.exit(-1)

    url = URL(sys.argv[1])

    # the secret is exactly 8 bytes.
    secret_len = 8

    # the server originally hashed:
    # secret || original command data
    original_data = url.suffix.encode()
    original_len = secret_len + len(original_data)

    # calculate the padding that sha256 added to the original message.
    original_padding = padding(original_len)

    # continue hashing from the internal state represented by the original token
    # the count tells pysha256 how many bytes have already been processed
    h = sha256(
        state=bytes.fromhex(url.token),
        count=original_len + len(original_padding)
    )

    # command we want to append
    suffix = b'&command=UnlockSafes'

    # length extension
    h.update(suffix)

    # replace the old token with the forged token
    url.token = h.hexdigest()

    # constructing:
    # original command || SHA-256 padding || new command
    forged_suffix = original_data + original_padding + suffix

    # need to keep the suffix in url format 
    url.suffix = quote(forged_suffix, safe='=&')

    print(url)


if __name__ == '__main__':
    main()
