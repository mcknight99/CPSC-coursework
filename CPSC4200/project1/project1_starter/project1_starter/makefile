solvecmd:
	python3 len_ext_attack.py "http://cpsc4200.mpese.com/spupke/lengthextension/api?token=499eb4b396f3c5b2b66a14fd0a22ac107605f737132ba7c2855291d68bebaa6c&command=SprinklersPowerOn"

hextobin:
	xxd -r -p file1.hex > file1.bin
	xxd -r -p file2.hex > file2.bin

# need to regenerate a few times due to the hash collisions in the blob (lol) but this does generate it quite quickly
# alternate approach i tried would have given guaranteed difference each time (check for first difference in hash bits)
# but that approach would have been a bit more painful to automate as a make script  
generate-good-and-evil:
	rm -f prefix suffix good.py goodgen.py evil.py col1 col2 gensuffix
	printf '%s\n' '#!/usr/bin/python3' '# coding: latin-1' 'blob = """' > prefix
	fastcoll -p prefix -o col1 col2

# generate a temporary Python program that computes the blob's SHA-256 and prints it as a quoted Python string for inserting into the suffix
	printf '%s\n' '' '"""' 'from hashlib import sha256' 'print("\"" + sha256(blob.encode("latin-1")).hexdigest() + "\"):", end="")' > gensuffix

# put the first collision blob into the temporary program and run it
	cat col1 gensuffix > goodgen.py

# generate the suffix with the correct hash for the good blob
	printf '%s\n' '' '"""' 'from hashlib import sha256' 'if(sha256(blob.encode("latin-1")).hexdigest() == ' > s1
	python3.7 goodgen.py > s2
	printf '%s\n' '' '    print("Use SHA-256 instead!")' 'else:' '    print("MD5 is perfectly secure!")' > s3
	cat s1 s2 s3 > suffix

	cat col1 suffix > good.py
	cat col2 suffix > evil.py

	openssl dgst -md5 good.py evil.py
	openssl dgst -sha256 good.py evil.py
	cat good.py
	cat evil.py
	python3.7 good.py
	python3.7 evil.py