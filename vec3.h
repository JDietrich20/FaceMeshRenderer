#pragma once
#include <cmath>

struct Vec3
{
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

    Vec3 operator+(const Vec3 &other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3 &other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
};

// Basic math for rotating points and projecting them
Vec3 rotateX(const Vec3 &v, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return {v.x, v.y * c - v.z * s, v.y * s + v.z * c};
}

Vec3 rotateY(const Vec3 &v, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return {v.x * c + v.z * s, v.y, -v.x * s + v.z * c};
}

// Perspective projection (manual)
void projectVertex(const Vec3 &v, float &x, float &y, int width, int height, float fov, float distance)
{
    float scale = fov / (distance + v.z); // perspective divide
    x = (v.x * scale) * width / 2 + width / 2;
    y = (v.y * scale) * height / 2 + height / 2;
}
