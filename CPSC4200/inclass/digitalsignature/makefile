generate-private:
	openssl genrsa -out key.pem 2048

compute-public:
	openssl rsa -in key.pem -outform PEM -pubout -out public.pem

calculate-signature:
	echo "a virus" > message.txt
	openssl dgst -sha256 -sign key.pem -out message.sig message.txt

verify-signature:
	openssl dgst -sha256 -verify public.pem -signature message.sig message.txt

encrypt-message-with-public-key:
	openssl pkeyutl -encrypt -pubin -inkey public.pem -in message.txt -out message.enc

encrypt-message-with-mert-key:
	openssl pkeyutl -encrypt -pubin -inkey mert.pem -in message.txt -out mertmessage.enc

decrypt-message-with-private-key:
	openssl pkeyutl -decrypt -inkey key.pem -in message.enc -out message.dec

# easy way to plaintext this ^ is to open in hex editor and copy all as base64


# digital signatures sign the hash of the message for efficiency and security, never the raw message itself
# use pkeyutl