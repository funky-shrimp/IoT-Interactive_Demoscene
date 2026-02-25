struct Edge {
  int start;
  int end;
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

// Based on your provided logic, we break the loops [0,1,2,3] into individual pairs
const Edge cubeEdges[] = {
    // Front Face
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    // Back Face
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    // Connecting lines (Front to Back)
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

int cubeVerticesNumber = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
int cubeEdgesNumber = sizeof(cubeEdges) / sizeof(cubeEdges[0]);