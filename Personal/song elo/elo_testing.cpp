#include "song.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

int gen_rand_int(int min, int max); // forward declaration

int main()
{
    //rewrite this to not use memory, but to re-reference the file each time and update the file each time
    //this also saves the results in case the program crashes
    std::vector<Song> songs;
    std::ifstream file("songs.csv");
    std::string line;
    while (std::getline(file, line))
    {
        std::string name = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        std::string artist = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        float elo = std::stof(line);
        line = line.substr(line.find(",") + 1);
        int pulls = std::stoi(line);
        songs.push_back(Song(name, artist, elo, pulls));
    }

    float result = 0;
    do
    {
        int rand1 = gen_rand_int(0, songs.size() - 1);
        Song *thissong = &songs[rand1];
        int rand2;
        Song *randomsong;
        do
        {
            rand2 = gen_rand_int(0, songs.size() - 1);
            randomsong = &songs[rand2];
        } while (thissong == randomsong);

        std::cout << "Song " << thissong->name << " vs " << randomsong->name;
        std::cout << "\t(or -1 to quit)" << std::endl;
        std::cout << "Enter 1 if " << thissong->name << " wins, 0 if " << randomsong->name << " wins: ";
        float result;
        std::cin >> result;
        if (result == -1)
        {
            break;
        }
        Song::rescore(*thissong, result, *randomsong, 1 - result, 50);
        std::cout << std::endl;
    } while (result != -1);


    // print the order of the songs by elo
    std::sort(songs.begin(), songs.end(), [](Song a, Song b) { return a.elo > b.elo; });
    for (int i = 0; i < songs.size(); i++)
    {
        std::cout << songs[i].name << " by " << songs[i].artist << " with elo " << songs[i].elo << std::endl;
    }

    // write the songs back to the file
    std::ofstream output("songs.csv", std::ios::trunc);
    for (int i = 0; i < songs.size(); i++)
    {
        output << songs[i].name << "," << songs[i].artist << "," << songs[i].elo << "," << songs[i].pulls << std::endl;
    }
    output.close();
    
    return 0;
}

int gen_rand_int(int min, int max)
{
    int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}