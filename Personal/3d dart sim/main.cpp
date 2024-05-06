#include <iostream>
#include <cmath>

using namespace std;

struct Point {
    double x, y, z;
};

struct Direction {
    double a, b, c;
};

double calculateTimeToHitXZPlane(double x0, double y0, double z0, double a, double b, double c, double vo) {
    // Calculate time to hit xz-plane using kinematic equations
    return (-y0 / b) + (vo * (b / sqrt(a * a + b * b + c * c)));
}

Point calculateLocationAtTime(double x0, double y0, double z0, double a, double b, double c, double vo, double t) {
    // Calculate location at a given time using kinematic equations
    Point p;
    p.x = x0 + vo * (a / sqrt(a * a + b * b + c * c)) * t;
    p.y = 0;
    p.z = z0 + vo * (c / sqrt(a * a + b * b + c * c)) * t;
    return p;
}

int main() {
    double x0, y0, z0, a, b, c, vo;
    
    // Input validation loop
    do {
        // Input
        cout << "Enter initial position (x0 y0 z0): ";
        cin >> x0 >> y0 >> z0;
        cout << "Enter direction (a b c): ";
        cin >> a >> b >> c;
        cout << "Enter initial speed (vo): ";
        cin >> vo;

        // Check if y0 is less than 0 and b is greater than 0
        if (y0 >= 0 || b <= 0) {
            cout << "Invalid input. y0 should be less than 0 and b should be greater than 0." << endl;
        }
    } while (y0 >= 0 || b <= 0); // Repeat until valid input is provided

    // Calculate time to hit xz-plane
    double t = calculateTimeToHitXZPlane(x0, y0, z0, a, b, c, vo);
    
    // Calculate location at time t
    Point hitLocation = calculateLocationAtTime(x0, y0, z0, a, b, c, vo, t);

    // Output
    cout << "Time to hit xz-plane: " << t << " seconds" << endl;
    cout << "Location at time t: (" << hitLocation.x << ", " << hitLocation.y << ", " << hitLocation.z << ")" << endl;

    return 0;
}
