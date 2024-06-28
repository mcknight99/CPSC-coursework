#ifndef SONG_H
#define SONG_H

#include <cmath>
#include <iostream>
#include <string>

class Song
{
public:
    std::string name;
    std::string artist;
    float elo;
    int pulls; // implemented to eventually pull songs with less pulls more often
    Song(std::string name = "UNNAMED", std::string artist = "UNSPECIFIED", float elo = 100, int pulls = 0)
    {
        this->name = name;
        this->artist = artist;
        this->elo = elo;
        this->pulls = pulls;
    }

    static void rescore(Song &a, float Sa, Song &b, float Sb, float K = 100)
    {
        // dividing by 480 according to Jeff Sonas
        std::cout << "Old elo of " << a.name << "=" << a.elo << " and " << b.name << "=" << b.elo << std::endl;
        float Ea = 1 / (1 + pow(10, (b.elo - a.elo) / 480));
        float Eb = 1 / (1 + pow(10, (a.elo - b.elo) / 480));

        std::cout << "Estimated probability of " << a.name << " winning=" << Ea << " and " << b.name << " winning=" << Eb << std::endl;

        a.elo = a.elo + K * (Sa - Ea);
        b.elo = b.elo + K * (Sb - Eb);
        a.pulls++;
        b.pulls++;

        if (a.elo < 0)
        {
            a.elo = 0;
        }
        if (b.elo < 0)
        {
            b.elo = 0;
        }

        std::cout << "New elo of " << a.name << "=" << a.elo << " and " << b.name << "=" << b.elo << std::endl;
    }
};

#endif // SONG_H