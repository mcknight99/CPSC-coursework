#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

#include "Course.h"

class Student {
    private:
        std::string name;
        std::vector<std::string> takenCourses;
        std::vector<std::string> recommendedCourses;
    public:
        Student() = default;
        Student(const std::string &n) : name(n) {}
        // Student(const std::string &n, const std::vector<std::string> &c) {
        //     name = n; 
        //     takenCourses = c;
        // }
        std::string getName() const;
        void addTakenCourse(const std::string &c);
        void addRecommendation(const std::string &c);
        std::vector<std::string> getTakenCourses() const;
        std::vector<std::string> getRecommendations() const;
};

#endif