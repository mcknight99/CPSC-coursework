// this program is used to remove duplicates from songs.csv
// and insert new songs from new_songs.csv into songs.csv without duplicates

// songs.csv should include new songs from new_songs.csv and have no duplicates

#include "song_tools.h"

int main()
{
    std::vector<Song> songs = readFile("songs.csv");

    std::vector<Song> newSongs = readFile("new_songs.csv");
    std::cout << "New songs size: " << newSongs.size() << std::endl;
    std::cout << "Inserting new songs..." << std::endl;

    for (size_t i = 0; i < newSongs.size(); ++i)
    {
        bool found = false;
        for (size_t j = 0; j < songs.size(); ++j)
        {
            if (newSongs[i].name == songs[j].name && newSongs[i].artist == songs[j].artist && newSongs[i].link == songs[j].link)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            songs.push_back(newSongs[i]);
        }
    }
    std::cout << "Songs size: " << songs.size() << std::endl;
    std::cout << "Removing duplicates..." << std::endl;
    songs = removeDuplicates(songs); // double checking, because sometimes duplicates already exist in songs.csv
    std::cout << "Songs size: " << songs.size() << std::endl;

    writeFile(songs, "songs.csv");
    std::cout << "Finished songs size: " << songs.size() << std::endl;

    std::cout<<"Songs with same artist and title but different links: " << std::endl;
    std::vector<Song> sameSongs = findYTduplicates(songs);
    for (size_t i = 0; i < sameSongs.size(); ++i)
    {
        std::cout << "\t" << sameSongs[i].name << " by " << sameSongs[i].artist << " with link https://youtu.be/" << sameSongs[i].link << "/" << std::endl;
        sameSongs[i].link = "https://youtu.be/" + sameSongs[i].link + "/";
    }
    writeFile(sameSongs, "same_songs.csv");


    std::cout << "Done!" << std::endl;
}
