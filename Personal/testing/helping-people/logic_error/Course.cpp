#include "Course.h"

std::string Course::getId() const {
    return id;
}

bool Course::comparator(const Course &a, const Course &b) {
    return a.id < b.id;
}


bool Course::operator==(const Course &other) const {
    return this->id==other.id;
}

bool Course::operator<(const Course &other) const {
    return this->id < other.id;
}