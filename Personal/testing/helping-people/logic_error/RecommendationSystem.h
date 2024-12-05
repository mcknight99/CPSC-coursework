#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Student.h"
#include "Course.h"

class RecommendationSystem {
    private: 
        std::map<std::string, Student> students;
        std::set<std::string> courses;
    public:
        RecommendationSystem(std::string &f) {
            loadData(f);
            for (const auto &pair : students) {
                generateRecommendationsForStudent(pair.first);
            }
        }
        void loadData(const std::string &filename);
        std::vector<std::string> generateRecommendationsForStudent(const std::string &studentName) const;
        void outputRecomendations(std::ostream &out) const;

        int getTotalStudents() const;
        int getTotalCourses() const;
        int getTotalRecommendations() const;
};

#endif