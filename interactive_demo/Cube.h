// Define a structure to represent a point in 3D space
struct Vertex {
    float x, y, z;
};

struct PointScreen {
  float x,y;
};

// 1. Vertices: Fixed-size array (8 corners of a cube)
const Vertex cubeVertices[8] = {
  { 0.25,  0.25,  0.25},
  {-0.25,  0.25,  0.25},
  {-0.25, -0.25,  0.25},
  { 0.25, -0.25,  0.25},
  { 0.25,  0.25, -0.25},
  {-0.25,  0.25, -0.25},
  {-0.25, -0.25, -0.25},
  { 0.25, -0.25, -0.25}
};

int cubeVerticesNumber = sizeof(cubeVertices) / sizeof(cubeVertices[0]);