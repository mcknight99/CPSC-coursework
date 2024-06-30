# Command to run the elo program using elo_testing.cpp
run:
	g++ -o elo_testing.stc elo_testing.cpp -Wall -Werror -Wpedantic -static-pie -shared -static-libgcc -static-libstdc++
	./elo_testing.e

# -pie or -static-pie 

# Command to import songs from ytmusicapi to songs.csv using importsongs.py
import:
	python importsongs.py

# Command to update the songs.csv file with the new song data (new_songs.csv) and remove duplicates
updateCSV:
	g++ -o update update.cpp -Wall -Werror -Wpedantic
	./update
	del update.exe
	del new_songs.csv