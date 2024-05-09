#ifndef PLANE_HPP
#define PLANE_HPP

#include "data_type.hpp"
#include "point.hpp"
#include "line.hpp"
#include <vector>

enum RelationType {
    COINCIDENT,
    IN_FRONT,
    BEHIND,
    SPANNING
};
std::ostream& operator<<(std::ostream& os, const RelationType& type) {
    switch (type) {
        case COINCIDENT: return os << "Coincident";
        case IN_FRONT: return os << "In front";
        case BEHIND: return os << "Behind";
        case SPANNING: return os << "Spanning";
    }
    return os;
}

class Plane {
private:
    Point3D p;
    Vector3D n;

public:
    Plane(const Point3D& point, const Vector3D& normal) : p(point), n(normal) {}

    Point3D getPoint() const { return p; }
    Vector3D getNormal() const { return n; }

    NType dist2Point(const Point3D& p) const;
    Point3D intersect(const Line& l) const;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p) {
        os << "Point: " << p.getPoint() << ", Normal: " << p.getNormal();
        return os;
    }
};

class Polygon {
private:
    std::vector<Point3D> vertices;

public:
    Polygon(const std::vector<Point3D>& vertices) : vertices(vertices) {}

    bool operator==(const Polygon& other) const {
        if (this->vertices.size() != other.vertices.size()) return false;

        for (size_t i = 0; i < vertices.size(); ++i) {
            if (!(this->vertices[i] == other.vertices[i])) return false;
        }
        return true;
    }

    Plane computePlane() const;
    bool contains(const Point3D& p) const;

    Point3D centroid() const;
    RelationType relationWithPlane(const Plane& plane) const;
    std::pair<Polygon, Polygon> split(const Plane& plane) const;

    friend std::ostream& operator<<(std::ostream& os, const Polygon& p) {
        os << "Vertices: ";
        for (const auto& vertex : p.vertices) {
            os << vertex << " ";
        }
        return os;
    }
};

// Plane
NType Plane::dist2Point(const Point3D& point) const {
    Vector3D vector(point - p);
    NType distance = vector.dotProduct(n.unit());
    
    return distance;
}

Point3D Plane::intersect(const Line& line) const {
    if (line.isParallel(n)) {
        throw std::invalid_argument("Line is parallel to plane");
    }

    Vector3D direction = line.getUnit();
    Point3D p0 = line.getPoint();

    NType t = (n.dotProduct(Vector3D(p0 - p))) / n.dotProduct(direction);
    Point3D intersection = p0 + Point3D(direction.getX() * t, direction.getY() * t, direction.getZ() * t);
    
    return intersection;
}

// Polygon
Plane Polygon::computePlane() const {
    Vector3D normal;
    for (size_t i = 0; i < vertices.size() - 2; ++i) {
        Vector3D v1(vertices[i] - vertices[i + 1]);
        Vector3D v2(vertices[i + 1] - vertices[i + 2]);

        if (v1.crossProduct(v2).mag() == NType(0)) {
            continue;
        }

        normal = v1.crossProduct(v2).unit();
        break;
    }

    return Plane(vertices[0], normal);
}

bool Polygon::contains(const Point3D& point) const {
    NType angleSum = 0;
    const NType TWO_PI = 6.283185307179586;

    for (size_t i = 0; i < vertices.size(); i++) {
        Vector3D v1(vertices[i] - point);
        Vector3D v2(vertices[(i + 1) % vertices.size()] - point);

        NType d1 = v1.mag();
        NType d2 = v2.mag();

        double angle = acos((v1.dotProduct(v2) / (d1 * d2)).getValue());
        NType angleValue(angle);

        angleSum += angleValue;
    }

    return NType::abs(angleSum) == TWO_PI;
}

Point3D Polygon::centroid() const {
    Point3D centroid;
    for (const auto& vertex : vertices) {
        centroid = centroid + vertex;
    }

    return centroid / NType(vertices.size());
}

RelationType Polygon::relationWithPlane(const Plane& plane) const {
    size_t frontCount = 0;
    size_t backCount = 0;
    
    for (const auto& vertex : vertices) {
        NType distance = plane.dist2Point(vertex);

        if (distance > NType(0)) {
            frontCount++;
        } else if (distance < NType(0)) {
            backCount++;
        }
    }

    if (frontCount == 0 && backCount == 0) {
        return COINCIDENT;
    } else if (frontCount == 0) {
        return BEHIND;
    } else if (backCount == 0) {
        return IN_FRONT;
    } else {
        return SPANNING;
    }
}

std::pair<Polygon, Polygon> Polygon::split(const Plane& plane) const {
    std::vector<Point3D> frontVertices;
    std::vector<Point3D> backVertices;

    for (size_t i = 0; i < vertices.size(); i++) {
        Point3D current = vertices[i];
        Point3D next = vertices[(i + 1) % vertices.size()];

        NType currentDistance = plane.dist2Point(current);
        NType nextDistance = plane.dist2Point(next);

        if (currentDistance < NType(0) && nextDistance < NType(0)) {
            backVertices.push_back(current);
        } else if (currentDistance > NType(0) && nextDistance > NType(0)) {
            frontVertices.push_back(current);
        } else if (currentDistance == NType(0) || nextDistance == NType(0)) {
            if (currentDistance > NType(0)) {
                frontVertices.push_back(current);
            } else {
                backVertices.push_back(current);
            }

            if (nextDistance > NType(0)) {
                frontVertices.push_back(next);
            } else {
                backVertices.push_back(next);
            }
        } else {
            Point3D intersection = plane.intersect(Line(current, next));

            if (currentDistance > NType(0)) {
                frontVertices.push_back(current);
                frontVertices.push_back(intersection);

                backVertices.push_back(intersection);
                backVertices.push_back(next);
            } else {
                backVertices.push_back(current);
                backVertices.push_back(intersection);

                frontVertices.push_back(intersection);
                frontVertices.push_back(next);
            }
        }
    }

    return std::make_pair(Polygon(frontVertices), Polygon(backVertices));
}

#endif // PLANE_HPP
