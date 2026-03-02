
# Intro

This is an Arduino Project made during the class "IoT" taught at [HEIG-VD](https://heig-vd.ch/) for Media Engineer.

The purpose of the project was to do Interactive [Demoscenes](https://fr.wikipedia.org/wiki/Sc%C3%A8ne_d%C3%A9mo), where the user can play with a 3D cube and experiment with the Plasma Effect.

# Mathematical concepts

Some mathematical concepts had to be learned in order to comprehend how you get from 3d to 2d. Thanks to [Tsoding Video | One Formula That Demystifies 3D Graphics](https://www.youtube.com/watch?v=qjWkNZ0SXfo&t=531s), the project was easy to achieve.

To go further, but it isn't necessary... Check on Geometry, Vectors, Scalar Multiplication and Weak Projection.

# Setting up the project
## Components

| Name                    | Type            |
| ---------------------- | --------------- |
| Joystick               | Analog          |
| 3x Potentiometer       | Analog          |
| Arduino Uno Rev3       | Microcontroller |
| Screen 1.44" 128x128px | Screen                |
| Enough wires |- | 

## Wiring

Made with [Fritzing](https://fritzing.org/).

![](attachment/Interactive_Demo-Wiring.png)

## Messing around

>Press on Joystick button to switch between Demos

### Cube

The default demo shown when starting is the cube. You can control its rotation on X and Y axis with the Joystick.

### Plasma

When the 3 potentiometers are set to 0, you can switch on different colors presets with Joystick Direction :
- Fire palette -> UP
- Ocean palette -> down
- Acid palette -> left
- Psychedelic palette -> right

You can add your custom palette by playing with the potentiometers dedicated for the colors Red Green and Blue. It requires some fine tuning to have a stable color palette.
