#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
    private:
        std::string id;
    public: 
        Course() = default;
        Course(const std::string &i) : id(i) {}
        std::string getId() const;
        bool comparator(const Course &a, const Course &b);
        bool operator==(const Course &other) const;
        bool operator<(const Course &other) const;
};

#endif