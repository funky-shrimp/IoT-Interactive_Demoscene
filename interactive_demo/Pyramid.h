#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid : public Object3D {
private:
    Vertex _vertices[5] = {
        {0, 0.25, 0},    // Top
        {-0.25, -0.25, 0.25},  // Base Front-Left
        {0.25, -0.25, 0.25},   // Base Front-Right
        {0.25, -0.25, -0.25},  // Base Back-Right
        {-0.25, -0.25, -0.25}  // Base Back-Left
    };
    
    Edge _edges[8] = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, // Sides
        {1, 2}, {2, 3}, {3, 4}, {4, 1}  // Base
    };

public:
    int getVertexCount() override { return 5; }
    int getEdgeCount() override { return 8; }
    Vertex* getVertices() override { return _vertices; }
    Edge* getEdges() override { return _edges; }
};

#endif