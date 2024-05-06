#include <iostream>
#include <cmath>

// Function to calculate f(t) = t * cos(t)
double f(double t) {
    return t * sin(t) * cos(t);
}

// Function to calculate g(t) = t * sin(t)
double g(double t) {
    return t * cos(t);
}

// Function to calculate distance between two points
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Function to find points on the parametric curve that form a square
void findSquarePoints(double t_start, double step_size) {
    double t = t_start;
    double x1, y1, x2, y2, x3, y3, x4, y4;
    double distance1, distance2, distance3, distance4, diagonal1, diagonal2;

    do {
        // Calculate coordinates for the first point
        x1 = f(t);
        y1 = g(t);

        // Calculate coordinates for the next three points
        x2 = f(t + step_size);
        y2 = g(t + step_size);

        x3 = f(t + 2 * step_size);
        y3 = g(t + 2 * step_size);

        x4 = f(t + 3 * step_size);
        y4 = g(t + 3 * step_size);

        // Calculate distances between consecutive points
        distance1 = distance(x1, y1, x2, y2);
        distance2 = distance(x2, y2, x3, y3);
        distance3 = distance(x3, y3, x4, y4);
        distance4 = distance(x4, y4, x1, y1);

        // Calculate distances between diagonals
        diagonal1 = distance(x1, y1, x3, y3);
        diagonal2 = distance(x2, y2, x4, y4);

        // Update t and step_size
        t += step_size;
        step_size *= 1.001; // Adjust step size slightly

    } while (fabs(distance1 - distance2) > 0.001 || fabs(distance2 - distance3) > 0.001 ||
             fabs(distance3 - distance4) > 0.001 || fabs(diagonal1 - diagonal2) > 0.001);

    // Output the coordinates of the four points
    std::cout << "Coordinates of the four points forming the square:" << std::endl;
    std::cout << "(" << x1 << ", " << y1 << ")" << std::endl;
    std::cout << "(" << x2 << ", " << y2 << ")" << std::endl;
    std::cout << "(" << x3 << ", " << y3 << ")" << std::endl;
    std::cout << "(" << x4 << ", " << y4 << ")" << std::endl;
    std::cout<< "t: " << t << std::endl;
    std::cout<< "t+step_size: " << t + step_size << std::endl;
    std::cout<< "t+2*step_size: " << t + 2 * step_size << std::endl;
    std::cout<< "t+3*step_size: " << t + 3 * step_size << std::endl;
    std::cout<< "distance1: " << distance1 << std::endl;
    std::cout<< "distance2: " << distance2 << std::endl;
    std::cout<< "distance3: " << distance3 << std::endl;
    std::cout<< "distance4: " << distance4 << std::endl;
    std::cout<< "diagonal1: " << diagonal1 << std::endl;
    std::cout<< "diagonal2: " << diagonal2 << std::endl;


}

int main() {
    // Choose a starting value of t and an initial step size
    double t_start = 0.0; // Starting value of t
    double step_size = 0.01; // Initial step size

    // Find and output the points on the parametric curve that form a square
    findSquarePoints(t_start, step_size);

    return 0;
}
