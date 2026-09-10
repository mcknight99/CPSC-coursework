blob = ""

from hashlib import sha256
if(sha256(blob.encode("latin-1")).hexdigest() == 
"01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b"):
    print("Use SHA-256 instead!")
else:
    print("MD5 is perfectly secure!")