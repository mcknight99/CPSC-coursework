#include "RecommendationSystem.h"
#include "Student.h"
#include "Course.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // std::string filename = argv[2];
    std::string filename = "sampleInput1.txt";
    RecommendationSystem recSys(filename);
    // recSys.loadData(argv[2]);
    recSys.outputRecomendations(std::cout);

    // Course c("CPSC1020");
    // Course a("CPSC2020");
    // std::cout << (a < c);
}