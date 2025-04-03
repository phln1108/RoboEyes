# FluxGarage RoboEyes Library
A fork of [FluxGarage RoboEyes library](https://github.com/FluxGarage/RoboEyes).

With the purpose to be used on Esp32 with Arduino Framework

## Introduction
Draws smoothly animated robot eyes on OLED displays, using the Adafruit GFX library. Robot eye shapes are configurable in terms of width, height, border radius and space between. Several different mood expressions (happy, tired, angry, default) and animations (autoblinker, idle, laughing, confused) are available. All state changes have smooth transitions and thus, complex animation sequences are easily feasible.

## Installation
### Arduino IDE
| Step | Instruction |
|------|-------------|
| 01   | Choose "Code > Download Zip" |
| 02   |In the Arduino IDE, navigate to "Sketch > Include Library > Add .ZIP Library" and select the downloaded file |

### PlatFormIO
- **Option 1:**
  | Step | Instruction |
  |------|-------------|
  | 01   | Choose "Code > Download Zip" |
  | 02   | Move the Zip file to your project's lib Folder |
  | 03   | Unzip it |
  | 04   | Delete the Zip File |
  | 05   | Install the dependencies (if not already installed) |

- **Option 2:**
  | Step | Instruction |
  |------|-------------|
  | 01   | Enter into your project's lib directory |
  | 02   | Clone the repo in there |
  | 03   | Install the dependencies (if not already installed) |

## Functions

### General
- **begin()** _(screen-width, screen-height, max framerate)_
- **update()** _update eyes drawings in the main loop, limited by max framerate as defined in begin()_
- **drawEyes()** _same as update(), but without the framerate limitation_
  
### Define Eye Shapes, all values in pixels
- **setWidth()** _(byte leftEye, byte rightEye)_
- **setHeight()** _(byte leftEye, byte rightEye)_
- **setBorderradius()** _(byte leftEye, byte rightEye)_
- **setSpacebetween()** _(int space) -> can also be negative_
- **setCyclops()** _(bool ON/OFF) -> if turned ON, robot has only on eye_

### Define Face Expressions (Mood, Curiosity, Eye-Position, Open/Close)
- **setMood()** _mood expression, can be TIRED, ANGRY, HAPPY, DEFAULT_
- **setPosition()** _cardinal directions, can be N, NE, E, SE, S, SW, W, NW, DEFAULT (default = horizontally and vertically centered)_
- **setCuriosity()** _(bool ON/OFF) -> when turned on, height of the outer eyes increases when moving to the very left or very right_
- **open()** _open both eyes -> open(1,0) opens left eye only_
- **close()** _close both eyes -> close(1,0) closes left eye only_

### Set Horizontal and/or Vertical Flicker
Alternately displaces the eyes in the defined amplitude in pixels:
- **setHFlicker()** _(bool ON/OFF, byte amplitude)_
- **setVFlicker()** _(bool ON/OFF, byte amplitude)_

### Play Prebuilt Oneshot Animations
- **anim_confused()** _confused -> eyes shaking left and right_
- **anim_laugh()** _laughing -> eyes shaking up and down_
- **blink()** _close and open both eyes_
- **blink(0,1)** _close and open right eye_

### Macro Animators
Blinks both eyes randomly:
- **setAutoblinker()** _(bool ON/OFF, int interval, int variation) -> turn on/off, set interval between each blink in full seconds, set range for additional random interval variation in full seconds_

Repositions both eyes randomly:
- **setIdleMode()** _(bool ON/OFF, int interval, int variation) -> turn on/off, set interval between each eye repositioning in full seconds, set range for additional random interval variation in full seconds_

