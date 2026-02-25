#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "Joystick.h"
#include "ProjectionManager.h"
#include "Cube.h"

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

//https://barth-dev.de/online/rgb565-color-picker/
//https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units
#define OBJECT_COLOR 0x101F  //BLUE in RGB565

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
jskValues oldJoystickValues;
bool hasJoystickChanged = false;


/* ------
    SCREEN
   ------ 
*/
const int FPS = 24;
const float deltaTime = 1.0f / FPS;

/* ------
    Cube
   ------ 
*/
PointScreen currentProjectedPoints[8];
PointScreen previousPoints[8];  //OPTIMIZATION : storing previous point of the cube to redraw them

/* ------
    Misc / To be deleted
   ------ 
*/

float deltaZ = 1;   //for z translate
double angleX = 0;  //for around Y axis
double angleY = 0;  //for around X axis


//Clear the screen
void clear() {
  screen.fillScreen(ST77XX_BLACK);
}

// OPTIMIZATION
// clearPoint is used to clear the point from previous loop
// so we don't have to clear the screen between each frame
void clearPoint(int x, int y) {
  screen.fillRect(x - POINT_SIZE / 2, y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE, ST77XX_BLACK);
}

//Draw a bigger "pixel" to be easily seen by eye
void point(PointScreen p) {
  screen.fillRect(p.x - POINT_SIZE / 2, p.y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE, ST77XX_WHITE);
}

// OPTIMIZATION
// Like clearPoint but for lines
void clearLine(int x1, int y1, int x2, int y2) {
  screen.drawLine(x1, y1, x2, y2, ST77XX_BLACK);
}

void line(int x1, int y1, int x2, int y2) {
  screen.drawLine(x1, y1, x2, y2, ST77XX_WHITE);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  screen.initR(INITR_144GREENTAB);
  screen.setRotation(0);
  screen.fillScreen(ST77XX_BLACK);  //make the screen black

  cwidth = screen.width();
  cheight = screen.height();

  //joystick.init();
}

void loop() {
  //deltaZ+=1*deltaTime; //distance travelled += speed*timeEllapsed
  //angle += PI * deltaTime;

  joystickValues = joystick.getValue();

  if (joystickValues.x == oldJoystickValues.x && joystickValues.y == oldJoystickValues.y) {
    hasJoystickChanged = false;
  } else {
    hasJoystickChanged = true;
  }

  // Analog values are 0-1023. Center is ~512.
  // We calculate the offset from the center and apply sensitivity.
  float dx = (joystickValues.x - 512) / 512.0f;
  float dy = (joystickValues.y - 512) / 512.0f;

  // Update angles based on stick position
  angleY += dx * 0.05f;
  angleX += dy * 0.05f;

  joystick.log();

  // ERASE ENTIRE OLD FRAME
  // We use the coordinates stored in previousPoints from the LAST frame
  if (hasJoystickChanged) {
    for (int i = 0; i < cubeEdgesNumber; i++) {
      int start = cubeEdges[i].start;
      int end = cubeEdges[i].end;

      clearLine(previousPoints[start].x, previousPoints[start].y,
                previousPoints[end].x, previousPoints[end].y);
    }
  }

  if (hasJoystickChanged) {
    //Loop and draw the cube vertices
    for (int i = 0; i < cubeVerticesNumber; i++) {
      Vertex v = cubeVertices[i];

      // Apply dual-axis rotation
      v = ProjectionManager::rotate_xz(v, angleY);  // Rotate around Y axis (horizontal stick movement)
      v = ProjectionManager::rotate_yz(v, angleX);  // Rotate around X axis (vertical stick movement)

      // Getting vertex coordinates for screen
      currentProjectedPoints[i] = ProjectionManager::getVertexForScreen(v, cwidth, cheight);

      //Draw the Vertex on screen
      //point(currentProjectedPoints[i]);
    }

    //Loop and draw the edges
    for (int i = 0; i < cubeEdgesNumber; i++) {
      int start = cubeEdges[i].start;
      int end = cubeEdges[i].end;

      line(currentProjectedPoints[start].x, currentProjectedPoints[start].y,
           currentProjectedPoints[end].x, currentProjectedPoints[end].y);
    }

    // Update memory for efficient erasing
    for (int i = 0; i < cubeVerticesNumber; i++) {
      previousPoints[i] = currentProjectedPoints[i];
    }
  }


  delay(1000 / FPS);
}
