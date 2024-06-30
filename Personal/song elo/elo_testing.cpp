#include "song.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>

// this program picks two random songs from songs.csv and has the user pick which one they like more
// the program randomly picks 5 songs and chooses the two lowest draw counts to compare to ensure that more songs are compared equally
// the program then uses the elo system to update the elo of the two songs
// the program rewrites the songs.csv file with the updated elo ratings to ensure that the elo ratings are saved every iteration
// the program will continue to run until the user decides to stop
// the program will also write out the top 100 songs to a file called top_songs.csv after every iteration

int gen_rand_int(int min, int max); // forward declaration
std::vector<Song> readFile(std::string filename);
void writeFile(std::vector<Song> songs, std::string filename);

int main()
{
    bool quit = false;
    while (!quit)
    {

        std::vector<Song> songs = readFile("songs.csv");
        std::vector<Song> randomSongs;
        for (int i = 0; i < 5; i++)
        {
            int rand = gen_rand_int(0, songs.size() - 1);
            if (std::find(randomSongs.begin(), randomSongs.end(), songs[rand]) == randomSongs.end())
            {
                randomSongs.push_back(songs[rand]);
            }
            else
            {
                i--; // if a duplicate is found, decrement i to ensure that 5 unique songs are picked
            }
        }

        std::sort(randomSongs.begin(), randomSongs.end(), [](Song a, Song b)
                  { return a.pulls < b.pulls; });

        Song a = randomSongs[0];
        Song b = randomSongs[1];

        std::cout << "\nWhich song do you like more?\tOr -1 to quit" << std::endl;
        std::cout << "1. " << a.toString() << std::endl;
        std::cout << "2. " << b.toString() << std::endl;

        int choice;
        std::cin >> choice;
        // ensure that the user picks a valid choice
        while (choice != 1 && choice != 2 && choice != -1)
        {
            std::cout << "Invalid choice. Please pick 1 or 2 or -1 to quit" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cin >> choice;
        }

        if (choice == -1)
        {
            quit = true;
            break;
        }
        // Rescore
        Song::rescore(a, choice == 1, b, choice == 2);

        // rather than using find for a vector, I just iterate through the vector and replace the song with the updated song
        // sometimes duplicates exist and this helps weed them out, as if they trickle up to the top 100, they take up two spots
        for (size_t i = 0; i < songs.size(); i++)
        {
            if (songs[i].name == a.name)
            {
                songs[i] = a;
            }
            if (songs[i].name == b.name)
            {
                songs[i] = b;
            }
        }

        writeFile(songs, "songs.csv");

        std::sort(songs.begin(), songs.end(), [](Song a, Song b)
                  { return a.elo > b.elo; });

        writeFile(songs, "top_100.csv");
    }
}

std::vector<Song> readFile(std::string filename)
{
    std::vector<Song> songs;
    std::ifstream file("songs.csv");
    std::string line;
    while (std::getline(file, line))
    {
        std::string name, artist, link;
        float elo;
        int pulls;
        std::stringstream ss(line);
        std::getline(ss, name, ',');
        std::getline(ss, artist, ',');
        ss >> elo;
        ss.ignore();
        ss >> pulls;
        ss.ignore();
        std::getline(ss, link, ',');
        songs.push_back(Song(name, artist, elo, pulls, link));
    }
    file.close();
    return songs;
}

void writeFile(std::vector<Song> songs, std::string filename)
{
    std::ofstream out(filename);
    for (size_t i = 0; i < songs.size(); i++)
    {
        out << songs[i] << std::endl;
    }
}

int gen_rand_int(int min, int max)
{
    int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}