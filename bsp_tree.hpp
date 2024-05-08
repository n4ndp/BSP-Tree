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
    // code here
}

const Polygon* BSPNode::detectCollision(const LineSegment& traceLine) const {
    // code here
    return nullptr;
}

// BSPTree
void BSPTree::insert(const Polygon& polygon) {
    // code here
}

#endif // BSP_HPP
