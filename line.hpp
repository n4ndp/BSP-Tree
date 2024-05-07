#ifndef LINE_HPP
#define LINE_HPP

#include "data_type.hpp"
#include "point.hpp"

class Line;
class Vector3D;
class LineSegment {
private:
    Point3D p1;
    Point3D p2;

public:
    LineSegment() : p1(), p2() {}
    LineSegment(Point3D p1, Point3D p2) : p1(p1), p2(p2) {}
    ~LineSegment() = default;

    Point3D getP1() const { return p1; }
    Point3D getP2() const { return p2; }

    void setP1(Point3D p1) { this->p1 = p1; }
    void setP2(Point3D p2) { this->p2 = p2; }

    bool operator==(const LineSegment& l) const {
        return p1 == l.p1 && p2 == l.p2;
    }

    bool operator!=(const LineSegment& l) const {
        return !(*this == l);
    }

    NType length() const;
    Line getLine() const;

    friend std::ostream& operator<<(std::ostream& os, const LineSegment& l) {
        os << "[" << l.p1 << " to " << l.p2 << "]";
        return os;
    }
};

class Vector3D : public Point3D {
public:
    Vector3D() : Point3D(0, 0, 0) {}
    Vector3D(NType x, NType y, NType z) : Point3D(x, y, z) {}
    Vector3D(const Point3D& p) : Point3D(p) {}
    ~Vector3D() = default;

    NType dotProduct(const Vector3D& v) const;
    NType dotProduct(const Point3D& p) const;

    Vector3D crossProduct(const Vector3D& v) const;

    Vector3D operator-() const;
    Vector3D operator*(const NType k) const;
    Vector3D operator/(const NType k) const;
    Vector3D operator+(const Vector3D& v) const;
    Vector3D operator-(const Vector3D& v) const;
    Vector3D& operator+=(const Vector3D& v);
    Vector3D& operator-=(const Vector3D& v);
    Vector3D& operator*=(NType k);
    Vector3D& operator/=(NType k);

    NType mag() const;
    Vector3D unit() const;
    void normalize();

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "{" << v.getX().getValue() << "," << v.getY().getValue() << "," << v.getZ().getValue() << "}";
        return os;
    }
};

class Line {
private:
    Point3D p;
    Vector3D v;

public:
    Line() : p(), v() {}
    Line(Point3D p, Vector3D v) : p(p), v(v.unit()) {}
    Line(Point3D p1, Point3D p2) : p(p1), v(Vector3D(p2 - p1).unit()) {}
    Line(const LineSegment& l) : p(l.getP1()), v(Vector3D(l.getP2() - l.getP1()).unit()) {}
    ~Line() = default;

    Point3D getPoint() const { return p; }
    Vector3D getUnit() const { return v; }

    void setPoint(Point3D p) { this->p = p; }
    void setVector(Vector3D v) { this->v = v.unit(); }

    bool isParallel(const Line& l) const;
    bool isParallel(const Vector3D& v) const;
    bool isParallel(const LineSegment& l) const;

    bool isOrthogonal(const Line& l) const;
    bool isOrthogonal(const Vector3D& v) const;
    bool isOrthogonal(const LineSegment& l) const;

    friend std::ostream& operator<<(std::ostream& os, const Line& l) {
        os << "P:" << l.p << " V:" << l.v;
        return os;
    }
};

// LineSegment
NType LineSegment::length() const { return p1.distance(p2); }
Line LineSegment::getLine() const { return Line(p1, p2); }

// Vector3D
NType Vector3D::dotProduct(const Vector3D& v) const {
    return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
}
NType Vector3D::dotProduct(const Point3D& p) const {
    return getX() * p.getX() + getY() * p.getY() + getZ() * p.getZ();
}

Vector3D Vector3D::crossProduct(const Vector3D& v) const {
    return Vector3D(
        getY() * v.getZ() - getZ() * v.getY(),
        getZ() * v.getX() - getX() * v.getZ(),
        getX() * v.getY() - getY() * v.getX()
    );
}

Vector3D Vector3D::operator-() const { return Vector3D(-getX(), -getY(), -getZ()); }
Vector3D Vector3D::operator*(const NType k) const { return Vector3D(getX() * k, getY() * k, getZ() * k); }
Vector3D Vector3D::operator/(const NType k) const { return Vector3D(getX() / k, getY() / k, getZ() / k); }
Vector3D Vector3D::operator+(const Vector3D& v) const { return Vector3D(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ()); }
Vector3D Vector3D::operator-(const Vector3D& v) const { return Vector3D(getX() - v.getX(), getY() - v.getY(), getZ() - v.getZ()); }
Vector3D& Vector3D::operator+=(const Vector3D& v) { setX(getX() + v.getX()); setY(getY() + v.getY()); setZ(getZ() + v.getZ()); return *this; }
Vector3D& Vector3D::operator-=(const Vector3D& v) { setX(getX() - v.getX()); setY(getY() - v.getY()); setZ(getZ() - v.getZ()); return *this; }
Vector3D& Vector3D::operator*=(NType k) { setX(getX() * k); setY(getY() * k); setZ(getZ() * k); return *this; }
Vector3D& Vector3D::operator/=(NType k) { setX(getX() / k); setY(getY() / k); setZ(getZ() / k); return *this; }

NType Vector3D::mag() const { return sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2)); }
Vector3D Vector3D::unit() const { return *this / mag(); }
void Vector3D::normalize() { *this /= mag(); }

// Line
bool Line::isParallel(const Line& l) const {
    Vector3D v1 = this->getUnit();
    Vector3D v2 = l.getUnit();

    NType crossProduct = v1.dotProduct(v2);
    return abs(crossProduct) == 1;
}

bool Line::isParallel(const Vector3D& v) const {
    NType crossProduct = this->getUnit().dotProduct(v);
    return abs(crossProduct) == 1;
}

bool Line::isParallel(const LineSegment& l) const {
    Vector3D v1 = this->getUnit();
    Vector3D v2 = l.getLine().getUnit();

    NType dotProduct = v1.dotProduct(v2);
    return abs(dotProduct) == 1;
}

bool Line::isOrthogonal(const Line& l) const {
    Vector3D v1 = this->getUnit();
    Vector3D v2 = l.getUnit();

    NType dotProduct = v1.dotProduct(v2);
    return dotProduct == 0;
}

bool Line::isOrthogonal(const Vector3D& v) const {
    NType dotProduct = this->getUnit().dotProduct(v);
    return dotProduct == 0;
}

bool Line::isOrthogonal(const LineSegment& l) const {
    Vector3D v1 = this->getUnit();
    Vector3D v2 = l.getLine().getUnit();

    NType dotProduct = v1.dotProduct(v2);
    return dotProduct == 0;
}

#endif // LINE_H
