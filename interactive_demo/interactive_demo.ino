#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "Joystick.h"

#include "Cube.h"

#define TFT_CS 10
#define TFT_DC  9
#define TFT_RST 8

//https://barth-dev.de/online/rgb565-color-picker/
//https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units
#define OBJECT_COLOR 0x101F //BLUE in RGB565

/* ------
    Screen
   ------ 
*/

Adafruit_ST7735 screen(TFT_CS, TFT_DC, TFT_RST);

int cwidth;  //c means canvas
int cheight;

const int POINT_SIZE = 2;

/* ------
    Joystick
   ------ 
*/
const int JOYSTICK_PINS[] = { 0, 1, 4 };  //Joystick (analog X, analog Y, digital BTN)
Joystick joystick(JOYSTICK_PINS);         // Object creation
jskValues joystickValues;                 // Structure for manipulating the input

/* ------
    SCREEN
   ------ 
*/
const int FPS = 24;
const float deltaTime = 1.0f/FPS;

/* ------
    Misc / To be deleted
   ------ 
*/
Vertex dummyV;
float deltaZ = 1;
double angle = 0;

/* ------
    Optimisation
   ------ 
*/
PointScreen previousPoints[8]; //storing previous point of the cube to redraw them

//Clear the screen
void clear() {
  screen.fillScreen(ST77XX_BLACK);
}

void clearPoint(int x,int y){
  screen.fillRect(x - POINT_SIZE / 2, y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE, ST77XX_BLACK);
}


//Draw a bigger "pixel" to be easily seen by eye
void point(PointScreen p) {
  screen.fillRect(p.x - POINT_SIZE / 2, p.y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE, ST77XX_WHITE);
}

//Adapt a 3d object coordinates to screen coordinates
PointScreen getPointToScreenCoordinate(PointScreen pointToProject) {
  PointScreen screenPt;

  //Because the p coordinates are on -1 to 1 coordinate system.
  // that's not the case in a screen coordinate
  screenPt.x = (pointToProject.x + 1.0f) / 2.0f * cwidth;
  screenPt.y = (1 - (pointToProject.y + 1.0f) / 2.0f) * cheight;

  return screenPt;
}

//Project 3d vertex to 2d
PointScreen project(Vertex vertexToProject) {

  PointScreen projectedVertex;

  projectedVertex.x = vertexToProject.x/vertexToProject.z;
  projectedVertex.y = vertexToProject.y/vertexToProject.z;

  return projectedVertex;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  screen.initR(INITR_144GREENTAB);
  screen.setRotation(0);
  screen.fillScreen(ST77XX_BLACK); //make the screen black

  cwidth = screen.width();
  cheight = screen.height();

  Serial.println("Screen width : " + (String)cwidth + " | Screen height : " + (String)cheight);

  dummyV.x = 0.5;
  dummyV.y = 0;
  dummyV.z = 1;

  //joystick.init();
  
}

//Translate a vertex by in z axis
Vertex translate_z(Vertex p, float dz){
  p.z+=dz;
  return p;
}

//Rotate along Y axis
Vertex rotate_xz(Vertex p, float angle){
  double c = cos(angle);
  double s = sin(angle);

  float oldX = p.x;
  p.x = p.x*c-p.z*s;
  p.z = oldX*s+p.z*c;

  return p;
}

void loop() {
  //deltaZ+=1*deltaTime; //distance travelled += speed*timeEllapsed
  angle += PI*deltaTime;

  for(int i = 0; i < cubeVerticesNumber; i++){
    //OPTIMIZATION : 
    clearPoint(previousPoints[i].x,previousPoints[i].y);

    // Transform
    Vertex transformedVertex = rotate_xz(cubeVertices[i], angle);

    //Project vertex from 3d to 2d
    // We add 1.5 to Z here to keep it visible and away from the camera
    Vertex forProjection = {transformedVertex.x, transformedVertex.y, transformedVertex.z + 1.5f};
    PointScreen projectedPoint = project(forProjection);

    //Convert to screen coordinates
    PointScreen currentPos = getPointToScreenCoordinate(projectedPoint);

    //Draw the Vertex on screen
    point(currentPos);

    //OPTIMIZATION
    previousPoints[i] = currentPos;
  }


  //point(getPointToScreenCoordinate(project(dummyV)));
  //joystickValues = joystick.getValue();

  //delay(1000/FPS);
  delay(1000/FPS);
}
