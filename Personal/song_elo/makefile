# Command to run the elo program using elo_testing.cpp
run:
	g++ -o elo_testing.exe elo_testing.cpp -Wall -Werror -Wpedantic 
	./elo_testing.exe

# Command to import songs from ytmusicapi to songs.csv using importsongs.py
import:
	python importsongs.py

# Command to update the songs.csv file with the new song data (new_songs.csv) and remove duplicates
updateCSV:
	g++ -o update update.cpp -Wall -Werror -Wpedantic
	./update
	del update.exe
	del new_songs.csv