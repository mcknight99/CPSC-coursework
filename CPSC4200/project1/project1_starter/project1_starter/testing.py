from pysha256 import sha256, padding
from urllib.parse import quote

sprinklers_token = "499eb4b396f3c5b2b66a14fd0a22ac107605f737132ba7c2855291d68bebaa6c"
sprinlkers_data = b"command=SprinklersPowerOn"

clock_token = "15a80ed015539ecc9b854e161f503870021dfc089583ca6868992c5098259c48"
clock_data = b"&command=ClockPowerOff&command=NoOp&command=ClockPowerOn"

# the secret is exactly 8 bytes
original_length_sprinlkers = 8 + len(sprinlkers_data)
original_length_clock = 8 + len(clock_data)

# sha256 padding originally added
padding_sprinklers = padding(original_length_sprinlkers)
padding_clock = padding(original_length_clock)

# continue sha256 from the internal state represented by the token
# count = original_length + len(padding) tells pysha256 to prevent to have already processed the secret, the data, and padding 
h_sprinklers = sha256(state=bytes.fromhex(sprinklers_token), count=original_length_sprinlkers + len(padding_sprinklers))
h_clock = sha256(state=bytes.fromhex(clock_token), count=original_length_clock + len(padding_clock))

# add the command we want to append
suffix_sprinklers = b"&command=SprinklersPowerOff"
suffix_clock = b"&command=ClockPowerOff&command=NoOp&command=ClockPowerOn&command=ClockPowerOff"

h_sprinklers.update(suffix_sprinklers)
h_clock.update(suffix_clock)

new_token_sprinklers = h_sprinklers.hexdigest()
new_token_clock = h_clock.hexdigest()