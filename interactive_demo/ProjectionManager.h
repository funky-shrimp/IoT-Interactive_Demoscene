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
    // We add 1.5 to Z here to keep it visible and away from the camera
    Vertex forProjection = { vertex.x, vertex.y, vertex.z + 1.0f };
    PointScreen projectedPoint = project(forProjection);
    //Convert to screen coordinates
    PointScreen vertexForScreen = getPointToScreenCoordinate(projectedPoint, screenWidth, screenHeight);

    return vertexForScreen;
  }

  //Adapt a 3d object coordinates to screen coordinates
  static PointScreen getPointToScreenCoordinate(PointScreen pointToProject, int screenWidth, int screenHeight) {
    PointScreen screenPt;

    //Because the p coordinates are on -1 to 1 coordinate system.
    // that's not the case in a screen coordinate
    screenPt.x = (pointToProject.x + 1.0f) / 2.0f * screenWidth;
    screenPt.y = (1 - (pointToProject.y + 1.0f) / 2.0f) * screenHeight;

    return screenPt;
  }

  //Project 3d vertex to 2d
  static PointScreen project(Vertex vertexToProject) {

    PointScreen projectedVertex;

    projectedVertex.x = vertexToProject.x / vertexToProject.z;
    projectedVertex.y = vertexToProject.y / vertexToProject.z;

    return projectedVertex;
  }

  //Translate a vertex in z axis
  static Vertex translate_z(Vertex p, float dz) {
    p.z += dz;
    return p;
  }

  //Rotate along Y axis
  static Vertex rotate_xz(Vertex p, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldX = p.x;
    p.x = p.x * c - p.z * s;
    p.z = oldX * s + p.z * c;

    return p;
  }

  //Rotate along x axis
  static Vertex rotate_yz(Vertex p, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldY = p.y;
    p.y = oldY * c - p.z * s;
    p.z = oldY * s + p.z * c;

    return p;
  }

  //Rotate along z axis
  static Vertex rotate_xy(Vertex p, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float oldX = p.x;
    p.x = oldX * c - p.y * s;
    p.y = oldX * s + p.y * c;

    return p;
  }
};

#endif