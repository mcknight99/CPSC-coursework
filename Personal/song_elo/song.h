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
    std::string link;
    Song(std::string name = "UNNAMED", std::string artist = "UNSPECIFIED", float elo = 100, int pulls = 0, std::string link = "UNSPECIFIED")
    {
        this->name = name;
        this->artist = artist;
        this->elo = elo;
        this->pulls = pulls;
        this->link = link;
    }

    static void rescore(Song &a, float Sa, Song &b, float Sb, float K = 100)
    {
        float oldEloA = a.elo;
        float oldEloB = b.elo;
        // dividing by 480 according to Jeff Sonas
        float Ea = 1 / (1 + pow(10, (b.elo - a.elo) / 480));
        float Eb = 1 / (1 + pow(10, (a.elo - b.elo) / 480));

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

        std::cout << a.name << " elo = " << oldEloA << "->" << a.elo << " and " << b.name << " elo = " << oldEloB << "->" << b.elo << std::endl;
        std::cout << a.name << " chance = " << Ea*100 << "% and " << b.name << " chance = " << Eb*100 << "%" << std::endl;

    }

    // friend function to overload the << operator ease of csv output
    friend std::ostream &operator<<(std::ostream &out, const Song &s)
    {
        out << s.name << "," << s.artist << "," << s.elo << "," << s.pulls << "," << s.link;
        return out;
    }

    std::string toString()
    {
        return name + " by " + artist + ". Link: https://youtu.be/" + link + "/";
    }

    friend bool operator==(const Song &a, const Song &b)
    {
        return a.name == b.name && a.artist == b.artist && a.link == b.link;
    }
};



#endif // SONG_H