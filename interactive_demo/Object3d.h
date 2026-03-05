// Object3D.h
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ProjectionManager.h" // Assuming this defines Vertex, etc.

// Define the maximum number of vertices any of your shapes will have
#define MAX_VERTICES 26

class Object3D {
public:
    // Buffers to store screen projections for optimization
    PointScreen currentProjectedPoints[MAX_VERTICES];
    PointScreen previousPoints[MAX_VERTICES];

    //Virtual function that have to be overriden by children
    virtual int getVertexCount() = 0;
    virtual int getEdgeCount() = 0;
    virtual Vertex* getVertices() = 0;
    virtual Edge* getEdges() = 0;
    
    // Virtual destructor is important when using polymorphism
    virtual ~Object3D() {}
};

#endif