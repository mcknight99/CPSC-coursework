#include "Student.h"

std::string Student::getName() const {
    return name;
}

void Student::addTakenCourse(const std::string &c) {
    takenCourses.push_back(c);
}

void Student::addRecommendation(const std::string &c) {
    recommendedCourses.push_back(c);
}

std::vector<std::string> Student::getTakenCourses() const {
    return takenCourses;
}

std::vector<std::string> Student::getRecommendations() const {
    return recommendedCourses;
}