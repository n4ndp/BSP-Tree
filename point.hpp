#ifndef POINT_HPP
#define POINT_HPP

#include "data_type.hpp"
#include <iostream>

class Point3D {
private:
    NType x, y, z;

public:
    Point3D() : x(0), y(0), z(0) {}
    Point3D(NType x, NType y, NType z) : x(x), y(y), z(z) {}
    ~Point3D() = default;

    NType getX() const { return x; }
    NType getY() const { return y; }
    NType getZ() const { return z; }

    void setX(NType x) { this->x = x; }
    void setY(NType y) { this->y = y; }
    void setZ(NType z) { this->z = z; }

    bool operator==(const Point3D& p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const Point3D& p) const {
        return !(*this == p);
    }

    Point3D operator-(const Point3D& p) const {
        return Point3D(x - p.x, y - p.y, z - p.z);
    }

    Point3D operator+(const Point3D& p) const {
        return Point3D(x + p.x, y + p.y, z + p.z);
    }

    Point3D operator/(const NType k) const {
        return Point3D(x / k, y / k, z / k);
    }

    NType distance(const Point3D& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Point3D& p) {
        os << "(" << p.x.getValue() << "," << p.y.getValue() << "," << p.z.getValue() << ")";
        return os;
    }
};

NType Point3D::distance(const Point3D& p) const {
    return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2));
}

#endif // POINT_HPP
