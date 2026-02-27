#ifndef PROJECTIONMANAGER_H
#define PROJECTIONMANAGER_H

// Define a structure to represent a point in 3D space
struct Vertex {
  float x, y, z;
};

struct PointScreen {
  float x, y;
};

class ProjectionManager {
private:
  static const int POINT_SIZE = 2;
public:

  //Function wrapper to get a Vertex ready to be printed
  // on a screen
  static PointScreen getVertexForScreen(Vertex vertex, int screenWidth, int screenHeight) {
    //Project vertex from 3d to 2d
    // We add Z offset here to keep it visible and away from the camera
    Vertex forProjection = { vertex.x, vertex.y, vertex.z + 1.0f };
    PointScreen projectedPoint = project(forProjection);

    //Convert to screen coordinates
    PointScreen vertexForScreen = getPointToScreenCoordinate(projectedPoint, screenWidth, screenHeight);

    return vertexForScreen;
  }

  //Adapt a 3d vertex coordinates to screen coordinates
  static PointScreen getPointToScreenCoordinate(PointScreen pointToConvert, int screenWidth, int screenHeight) {
    PointScreen screenPt;

    //Because the pointToProject coordinates are on -1 to 1 coordinate system.
    // we have to adapt it's coordinate to be in a 0 to 1 coordinate system.
    screenPt.x = (pointToConvert.x + 1.0f) / 2.0f * screenWidth;
    screenPt.y = (1 - (pointToConvert.y + 1.0f) / 2.0f) * screenHeight;

    return screenPt;
  }

  //Project a 3d vertex to 2d with Weak Perspective
  static PointScreen project(Vertex vertexToProject) {

    PointScreen projectedVertex;

    projectedVertex.x = vertexToProject.x / vertexToProject.z;
    projectedVertex.y = vertexToProject.y / vertexToProject.z;

    return projectedVertex;
  }

  //Translate a vertex along z axis
  static Vertex translate_z(Vertex vertex, float dz) {
    vertex.z += dz;
    return vertex;
  }

  //Rotate along Y axis
  static Vertex rotate_xz(Vertex vertex, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldX = vertex.x;
    vertex.x = vertex.x * c - vertex.z * s;
    vertex.z = oldX * s + vertex.z * c;

    return vertex;
  }

  //Rotate along x axis
  static Vertex rotate_yz(Vertex vertex, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldY = vertex.y;
    vertex.y = oldY * c - vertex.z * s;
    vertex.z = oldY * s + vertex.z * c;

    return vertex;
  }

  //Rotate along z axis
  static Vertex rotate_xy(Vertex vertex, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldX = vertex.x;
    vertex.x = oldX * c - vertex.y * s;
    vertex.y = oldX * s + vertex.y * c;

    return vertex;
  }
};

#endif