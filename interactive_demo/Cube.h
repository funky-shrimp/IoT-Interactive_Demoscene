struct Edge {
  int start;
  int end;
};

// 1. Vertices: Fixed-size array (8 corners of a cube)
const Vertex cubeVertices[8] = {
  { 0.25,  0.25,  0.25}, //0
  {-0.25,  0.25,  0.25}, //1
  {-0.25, -0.25,  0.25}, //2
  { 0.25, -0.25,  0.25}, //3
  { 0.25,  0.25, -0.25}, //4
  {-0.25,  0.25, -0.25}, //5
  {-0.25, -0.25, -0.25}, //6
  { 0.25, -0.25, -0.25}  //7
};

// Array of edges indicating wich Vertices are connected together
// e.g Vertex 0 is connected to Vertex 1
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