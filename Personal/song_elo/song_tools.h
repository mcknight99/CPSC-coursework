#ifndef UPDATE_H
#define UPDATE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include "song.h"


std::vector<Song> readFile(std::string filename)
{
    std::vector<Song> songs;
    std::ifstream file(filename);
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
    out.close();
}

// if duplicates do exist, the one that has the most pulls will be kept as it is the most accurate representation of the song's score
std::vector<Song> removeDuplicates(std::vector<Song> songs)
{
    std::vector<Song> uniqueSongs;
    for (size_t i = 0; i < songs.size(); ++i)
    {
        bool found = false;
        for (size_t j = 0; j < uniqueSongs.size(); ++j)
        {
            if (songs[i].name == uniqueSongs[j].name && songs[i].artist == uniqueSongs[j].artist && songs[i].link == uniqueSongs[j].link)
            {
                found = true;
                if (songs[i].pulls > uniqueSongs[j].pulls)
                {
                    uniqueSongs[j] = songs[i];
                }
                break;
            }
        }
        if (!found)
        {
            uniqueSongs.push_back(songs[i]);
        }
    }
    return uniqueSongs;
}

// ytduplicates are songs with identical titles and artists, but different links
std::vector<Song> findYTduplicates(std::vector<Song> songs)
{
    std::vector<Song> duplicates;
    for (size_t i = 0; i < songs.size(); ++i)
    {
        for (size_t j = i + 1; j < songs.size(); ++j)
        {
            if (songs[i].name == songs[j].name && songs[i].artist == songs[j].artist && songs[i].link != songs[j].link)
            {
                duplicates.push_back(songs[i]);
                duplicates.push_back(songs[j]);
            }
        }
    }
    return duplicates;
}

#endif // UPDATE_H