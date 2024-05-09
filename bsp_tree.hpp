#ifndef BSP_HPP
#define BSP_HPP

#include "data_type.hpp"
#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include <vector>

class BSPNode {
public:
    BSPNode* front;
    BSPNode* back;
    BSPNode* parent;
    Plane partition;
    std::vector<Polygon> polygons;

    BSPNode(const Plane& partition) : partition(partition), front(nullptr), back(nullptr), parent(nullptr) {}
    ~BSPNode() {
        delete front;
        delete back;
    }

    void insert(const Polygon& polygon);

    const Polygon* detectCollision(const LineSegment& traceLine) const;
    
    size_t getPolygonsCount() const {
        size_t count = polygons.size();
        if (front) {
            count += front->getPolygonsCount();
        }
        if (back) {
            count += back->getPolygonsCount();
        }
        return count;
    }
};

class BSPTree {
private:
    BSPNode* root;

public:
    BSPTree() : root(nullptr) {}
    ~BSPTree() {
        delete root;
    }

    BSPNode* getRoot() const { return root; }
    bool isEmpty() const { return root == nullptr; }

    void insert(const Polygon& polygon);

    const Polygon* detectCollision(const LineSegment& line) const {
        if (root == nullptr) {
            return nullptr;
        }
        return root->detectCollision(line);
    }

    size_t getRootPolygonsCount() const { return root ? root->polygons.size() : 0; }
    size_t getPolygonsCount() const {
        if (root == nullptr) {
            return 0;
        } else {
            return root->getPolygonsCount();
        }
    }
};

// BSPNode
void BSPNode::insert(const Polygon& polygon) {
    RelationType relation = polygon.relationWithPlane(partition);
    Plane plane = polygon.computePlane();

    if(relation == RelationType::IN_FRONT) {
        if (front == nullptr) {
            front = new BSPNode(plane);
            front->parent = this;
            front->polygons.push_back(polygon);
        } else {
            front->insert(polygon);
        }
    } else if(relation == RelationType::BEHIND) {
        if (back == nullptr) {
            back = new BSPNode(plane);
            back->parent = this;
            back->polygons.push_back(polygon);
        } else {
            back->insert(polygon);
        }
    } else if(relation == RelationType::COINCIDENT) {
        polygons.push_back(polygon);
    } else if(relation == RelationType::SPANNING) {
        auto [frontPoly, backPoly] = polygon.split(plane);

        if (front == nullptr) {
            front = new BSPNode(plane);
            front->parent = this;
            front ->polygons.push_back(frontPoly);
        } else {
            front->insert(frontPoly);
        }

        if (back == nullptr) {
            back = new BSPNode(plane);
            back->parent = this;
            back->polygons.push_back(backPoly);
        } else {
            back->insert(backPoly);
        }
    } else {
        throw std::runtime_error("Invalid relation type");
    }
}

const Polygon* BSPNode::detectCollision(const LineSegment& traceLine) const {
    
}

// BSPTree
void BSPTree::insert(const Polygon& polygon) {
    if (root == nullptr) {
        root = new BSPNode(polygon.computePlane());
        root->polygons.push_back(polygon);
    } else {
        root->insert(polygon);
    }
}

#endif // BSP_HPP
