#include "RecommendationSystem.h"

void RecommendationSystem::loadData(const std::string &filename) {
    // open file
    std::ifstream file(filename);
    std::string line, name, course;
    std::stringstream ss;

    while (getline(file, line))
    {
        // read student names
        std::stringstream ss(line);
        getline(ss, name, ':');
        // add to map and declare student objects
        Student newSt(name);
        students[name] = newSt;
        std::cout << name << '\t';

        // read in classes and add in classes to set
        while (getline(ss, course, ',')) {
            // course = course.substr(0, course.length());
            courses.insert(course);
            students.at(name).addTakenCourse(course);
            std::cout << course << '\t';
        }
        std::cout << std::endl;
    }
    file.close();
}

std::vector<std::string> RecommendationSystem::generateRecommendationsForStudent(const std::string &studentName) const {
    // FOR ALL COURSES
    std::cout<< "Generating recommendations for " << studentName << std::endl;
    Student st = students.at(studentName);
    std::vector<std::string> st_courses = st.getTakenCourses();

    std::map<std::string, int> rec_courses;
    std::vector<std::string> finalRecs;

    std::vector<std::string> allCourses(courses.begin(), courses.end());

    // FOR ALL COURSES
    // for (const auto &c_temp : st_courses) {

    // }
    
    for (size_t ii = 0; ii < allCourses.size(); ii++) {
        // check if course is not in student list 
        if (find(st_courses.begin(), st_courses.end(), allCourses[ii]) == st_courses.end()) {
            // check if already existing in rec list
            rec_courses[allCourses[ii]]++;
        }
    }
    
    std::map<int, std::vector<Course>> rev_recs;
    for (const auto &pair : rec_courses) {
        rev_recs[pair.second].push_back(Course(pair.first));
    } // now ordered by weight and converted to Course type

    // for all courses in reversed rec map
    //int ii = 0;
    for (const auto &pair : rev_recs) {
        std::vector<Course> newRecs = pair.second;
        if (newRecs.size() > 1) {
            std::sort(newRecs.begin(), newRecs.end());
        } 

        for (const Course &r : newRecs) {
            finalRecs.push_back(r.getId());
        }
    }
    // order by weight
    // if weight is the same
    // order by alpha (use courses)

    // add to student rec list
    for (const std::string &c : finalRecs) {
        st.addRecommendation(c);
    }
    // // return string list
    return finalRecs;
    // std::vector<std::string> bloop;
    // return bloop;
}

void RecommendationSystem::outputRecomendations(std::ostream &out) const {
    for (const auto &s : students) {
        out << s.first << ": " << "[";
        std::vector<std::string> recs = s.second.getRecommendations();
        std::vector<std::string>::iterator it = recs.begin();
        while (it < recs.end()) {
            out << *it++ << ", ";
        }
        out << *it << "]" << std::endl;
    }
}


int RecommendationSystem::getTotalStudents() const {
    return students.size();
}

int RecommendationSystem::getTotalCourses() const {
    return courses.size();
}

int RecommendationSystem::getTotalRecommendations() const {
    int total = 0;
    for (const auto &s : students) {
        total += s.second.getRecommendations().size();
    }
    return total;
}