#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "Joystick.h"
#include "ProjectionManager.h"
#include "Cube.h"
#include "Plasma.h"

#define TFT_CS 10  //for screen
#define TFT_DC 9   //for screen
#define TFT_RST 8  //for screen

//https://barth-dev.de/online/rgb565-color-picker/
//https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units
#define OBJECT_COLOR 0x101F  //BLUE in RGB565

/* ------
    Screen
   ------ 
*/
Adafruit_ST7735 screen(TFT_CS, TFT_DC, TFT_RST);

int cwidth;  //c means canvas / screen
int cheight;

const int POINT_SIZE = 2;  //To draw a "pixel" based on a vertex position

const int FPS = 24;

/* ------
    Joystick
   ------ 
*/
const int JOYSTICK_PINS[] = { 0, 1, 4 };  // Joystick (analog X, analog Y, digital BTN)
Joystick joystick(JOYSTICK_PINS);         // Object creation
jskValues joystickValues;                 // Structure for manipulating the input
bool hasJoystickChanged = false;          // OPTIMIZATION : we only draw the cube if the Joystick has a movement
const int JOYSTICK_TOLERANCE = 20;        //Because a Joystick is not always perfectly centered


/* ------
    Plasma effect
   ------ 
*/
PlasmaEffect plasma(screen);

/* ------
    Demos
   ------ 
*/
bool demoSwitch = true;
int lastButtonState = 1;  // 1 is the "unpressed" state for your pull-up configuration

/* ------
    Cube / 3D object
   ------ 
*/
PointScreen currentProjectedPoints[8];
PointScreen previousPoints[8];  //OPTIMIZATION : storing previous point of the cube to redraw them

const float ROTATION_SENSITIVITY = 0.3f;

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
  Serial.begin(9600);  //For Serial connection

  screen.initR(INITR_144GREENTAB);
  screen.setRotation(0);
  screen.fillScreen(ST77XX_BLACK);  //make the screen black

  plasma.setPalette(FIRE);
  plasma.setResolution(4);  // 4 for speed, 2 for beauty

  cwidth = screen.width();
  cheight = screen.height();

  joystick.init();
}

void loop() {
  // to draw the cube on the first frame
  // if joystick is "resting"
  static bool firstFrame = true;

  // Analog values are 0-1023. Center is ~512.
  joystickValues = joystick.getValue();

  // We calculate the offset from the center and apply sensitivity.
  float dx = 0;
  float dy = 0;

  // Check X-axis with tolerance, because a Joystick isn't perfectly centered
  if (abs(joystickValues.x - 512) > JOYSTICK_TOLERANCE) {
    // Map the value to a -1.0 to 1.0 range based on the x center
    dx = (joystickValues.x - 512) / 512.0f;
  }

  // Check Y-axis with tolerance
  if (abs(joystickValues.y - 512) > JOYSTICK_TOLERANCE) {
    dy = (joystickValues.y - 512) / 512.0f;
  }

  //OPTIMIZATION to avoid redrawing the cube if not necessary.
  if (dx != 0 || dy != 0 || firstFrame) {
    hasJoystickChanged = true;

    // Only update angles if there is actual movement
    angleY += dx * ROTATION_SENSITIVITY;  //Joystick on joystick's X axis acts on Y axis of the cube
    angleX += dy * ROTATION_SENSITIVITY;

    // Once we have drawn the first frame, turn this off
    firstFrame = false;
  } else {
    hasJoystickChanged = false;
  }

  // Check if the button was JUST pressed
  if (joystickValues.btn == 0 && lastButtonState == 1) {
    demoSwitch = !demoSwitch;

    // Clear screen when switching to avoid leftover pixels from the other demo
    screen.fillScreen(ST77XX_BLACK);
  }

  // Update the last state for the next frame
  lastButtonState = joystickValues.btn;


  if (demoSwitch) {

    // OPTIMIZATION : Instead of clearing all the screen,
    // it just clears the previous frame of the cube (if joystick has moved)
    if (hasJoystickChanged) {
      for (int i = 0; i < cubeEdgesNumber; i++) {
        int start = cubeEdges[i].start;
        int end = cubeEdges[i].end;

        //Clear the cube edges
        clearLine(previousPoints[start].x, previousPoints[start].y,
                  previousPoints[end].x, previousPoints[end].y);
      }
    }

    //Draw the 3d object if joystick has changed
    if (hasJoystickChanged) {
      //Loop and draw the cube vertices
      for (int i = 0; i < cubeVerticesNumber; i++) {
        Vertex vertex = cubeVertices[i];

        // Apply dual-axis rotation
        vertex = ProjectionManager::rotate_xz(vertex, angleY);  // Rotate around Y axis (horizontal stick movement)
        vertex = ProjectionManager::rotate_yz(vertex, angleX);  // Rotate around X axis (vertical stick movement)

        // Getting vertex coordinates for screen
        currentProjectedPoints[i] = ProjectionManager::getVertexForScreen(vertex, cwidth, cheight);

        //Draw the Vertex on screen
        //point(currentProjectedPoints[i]);
      }

      //Loop and draw the edges
      for (int i = 0; i < cubeEdgesNumber; i++) {
        int start = cubeEdges[i].start;  //retrieve the first vertex we want to connect
        int end = cubeEdges[i].end;      //retrieve the second vertex we want to connect iwht

        line(currentProjectedPoints[start].x, currentProjectedPoints[start].y,
             currentProjectedPoints[end].x, currentProjectedPoints[end].y);
      }

      // Update memory for efficient erasing
      for (int i = 0; i < cubeVerticesNumber; i++) {
        previousPoints[i] = currentProjectedPoints[i];
      }
    }
  } else {
    if (abs(dy) > 0.5) {
      PlasmaPalette palette = (dy > 0) ? FIRE : OCEAN;
      plasma.setPalette(palette);
    } else if (abs(dx) > 0.5) {
      PlasmaPalette palette = (dx > 0) ? ACID : PSYCHEDELIC;
      plasma.setPalette(palette);
    }

    //Because if the cube is static when switching to Plasma Effect
    // and coming back to Cube, the cube won't be drawn
    firstFrame = true;

    plasma.update();
  }

  delay(1000 / FPS);
}
