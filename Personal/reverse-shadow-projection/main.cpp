#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator*(float s) const {
        return Vec3(x * s, y * s, z * s);
    }

    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        return Vec3(x / len, y / len, z / len);
    }

    std::string key() const {
        int xi = (int)std::round(x);
        int yi = (int)std::round(y);
        int zi = (int)std::round(z);
        return std::to_string(xi) + "," + std::to_string(yi) + "," + std::to_string(zi);
    }
};

struct Voxel {
    Vec3 center;
};

std::vector<Voxel> voxels;
std::unordered_set<std::string> voxelSet;

void addVoxel(const Vec3& pos) {
    std::string k = pos.key();
    if (voxelSet.count(k)) return;
    voxelSet.insert(k);
    voxels.push_back({ pos });
}

// Generate cube voxels as STL
void writeSTL(const std::string& filename, float cubeSize) {
    std::ofstream out(filename);
    out << "solid voxels\n";

    Vec3 cubeVerts[8] = {
        {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
        {-1,-1, 1}, {1,-1, 1}, {1,1, 1}, {-1,1, 1}
    };

    int faces[6][4] = {
        {0,1,2,3}, {7,6,5,4}, // front/back
        {0,4,5,1}, {3,2,6,7}, // left/right
        {1,5,6,2}, {4,0,3,7}  // bottom/top
    };

    for (const auto& v : voxels) {
        for (int f = 0; f < 6; f++) {
            Vec3 v0 = cubeVerts[faces[f][0]] * (cubeSize/2) + v.center;
            Vec3 v1 = cubeVerts[faces[f][1]] * (cubeSize/2) + v.center;
            Vec3 v2 = cubeVerts[faces[f][2]] * (cubeSize/2) + v.center;
            Vec3 v3 = cubeVerts[faces[f][3]] * (cubeSize/2) + v.center;

            // Write two triangles
            out << "facet normal 0 0 0\n outer loop\n";
            out << "vertex " << v0.x << " " << v0.y << " " << v0.z << "\n";
            out << "vertex " << v1.x << " " << v1.y << " " << v1.z << "\n";
            out << "vertex " << v2.x << " " << v2.y << " " << v2.z << "\n";
            out << "endloop\nendfacet\n";

            out << "facet normal 0 0 0\n outer loop\n";
            out << "vertex " << v2.x << " " << v2.y << " " << v2.z << "\n";
            out << "vertex " << v3.x << " " << v3.y << " " << v3.z << "\n";
            out << "vertex " << v0.x << " " << v0.y << " " << v0.z << "\n";
            out << "endloop\nendfacet\n";
        }
    }

    out << "endsolid voxels\n";
    out.close();
}

// Returns the first intersection of ray from origin in dir with a sphere at center, radius r
bool raySphereIntersect(const Vec3& origin, const Vec3& dir, const Vec3& center, float radius, Vec3& hitPoint) {
    Vec3 oc = origin - center;
    float a = dir.dot(dir);
    float b = 2.0 * oc.dot(dir);
    float c = oc.dot(oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) return false;

    float t = (-b + std::sqrt(discriminant)) / (2.0 * a);  // take far side (outer shell)
    hitPoint = origin + dir * t;
    return true;
}

int main() {
    int width, height, channels;
    unsigned char* data = stbi_load("shadow-image.png", &width, &height, &channels, 1);
    if (!data) {
        std::cerr << "Failed to load image.\n";
        return 1;
    }

    // Settings
    Vec3 light = { 0, 0, 100 };     // light source above the sphere
    float sphereDiameter = 100.0f;
    float voxelSize = 1.5f;

    // Loop over image pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char pixel = data[y * width + x];
            if (pixel < 128) { // black pixel only
                // map pixel to world coordinates
                float fx = x - width / 2.0f;
                float fy = y - height / 2.0f;
                Vec3 imagePt = { fx, fy, 0 };

                Vec3 dir = (imagePt - light).normalize();
                Vec3 hit;
                if (raySphereIntersect(light, dir, {0,0,0}, sphereDiameter/2, hit)) {
                    addVoxel(hit);
                }
            }
        }
    }

    stbi_image_free(data);

    writeSTL("shell_shadow.stl", voxelSize);
    std::cout << "STL written with " << voxels.size() << " voxels.\n";

    return 0;
}
