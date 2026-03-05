#ifndef CUBE_H
#define CUBE_H

class Cube : public Object3D {
private:
Vertex _vertices[8] = {
  { 0.25,  0.25,  0.25}, //0
  {-0.25,  0.25,  0.25}, //1
  {-0.25, -0.25,  0.25}, //2
  { 0.25, -0.25,  0.25}, //3
  { 0.25,  0.25, -0.25}, //4
  {-0.25,  0.25, -0.25}, //5
  {-0.25, -0.25, -0.25}, //6
  { 0.25, -0.25, -0.25}  //7
};

Edge _edges[12] = {
    // Front Face
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    // Back Face
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    // Connecting lines (Front to Back)
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

public:
    int getVertexCount() override { return 8; }
    int getEdgeCount() override { return 12; }
    Vertex* getVertices() override { return _vertices; }
    Edge* getEdges() override { return _edges; }
};

#endif
