#include "RoboEyes.hpp"

//*********************************************************************************************
//  GENERAL METHODS
//*********************************************************************************************

RoboEyes::RoboEyes(int width, int height, byte frameRate, Adafruit_SSD1306* oled)
    : display(oled),
      screenWidth(width),
      screenHeight(height) {
    display->clearDisplay();
    display->display();
    setFramerate(frameRate);

    // Initialize LEFT eye (eyeL)
    eyeL = {
        // Eye width
        .widthDefault = EYE_WIDTH,
        .widthCurrent = 1,  // Start closed
        .widthNext = EYE_WIDTH,
        // Eye height
        .heightDefault = EYE_HEIGHT,
        .heightCurrent = 1,  // Start closed
        .heightNext = EYE_HEIGHT,
        .heightOffset = 0,
        // Border radius
        .borderRadiusDefault = EYE_BORDER_RADIUS,
        .borderRadiusCurrent = EYE_BORDER_RADIUS,
        .borderRadiusNext = EYE_BORDER_RADIUS,
        // Coordinates
        .xDefault = ((screenWidth) - (EYE_WIDTH + EYE_SPACE_BETWEEN + EYE_WIDTH)) / 2,
        .x = ((screenWidth) - (EYE_WIDTH + EYE_SPACE_BETWEEN + EYE_WIDTH)) / 2,
        .xNext = ((screenWidth) - (EYE_WIDTH + EYE_SPACE_BETWEEN + EYE_WIDTH)) / 2,
        .yDefault = ((screenHeight - EYE_HEIGHT) / 2),
        .y = ((screenHeight - EYE_HEIGHT) / 2),
        .yNext = ((screenHeight - EYE_HEIGHT) / 2)};

    // Initialize RIGHT eye (eyeR)
    eyeR = {
        // Eye width
        .widthDefault = EYE_WIDTH,
        .widthCurrent = 1,  // Start closed
        .widthNext = EYE_WIDTH,
        // ye height
        .heightDefault = EYE_HEIGHT,
        .heightCurrent = 1,  // Start closed
        .heightNext = EYE_HEIGHT,
        .heightOffset = 0,
        // order radius
        .borderRadiusDefault = EYE_BORDER_RADIUS,
        .borderRadiusCurrent = EYE_BORDER_RADIUS,
        .borderRadiusNext = EYE_BORDER_RADIUS,
        // oordinates
        .xDefault = eyeL.xDefault + EYE_WIDTH + EYE_SPACE_BETWEEN,
        .x = eyeL.x + EYE_WIDTH + EYE_SPACE_BETWEEN,
        .xNext = eyeL.xNext + EYE_WIDTH + EYE_SPACE_BETWEEN,
        .yDefault = eyeL.yDefault,
        .y = eyeL.y,
        .yNext = eyeL.yNext};
}

void RoboEyes::update() {
    // Limit drawing updates to defined max framerate
    if (millis() - fpsTimer >= frameInterval) {
        drawEyes();
        fpsTimer = millis();
    }
}

//*********************************************************************************************
//  SETTERS METHODS
//*********************************************************************************************

void RoboEyes::setFramerate(byte fps) {
    frameInterval = 1000 / fps;
}

void RoboEyes::setWidth(byte leftEye, byte rightEye) {
    eyeL.widthNext = leftEye;
    eyeR.widthNext = rightEye;
    eyeL.widthDefault = leftEye;
    eyeR.widthDefault = rightEye;
}

void RoboEyes::setHeight(byte leftEye, byte rightEye) {
    eyeL.heightNext = leftEye;
    eyeR.heightNext = rightEye;
    eyeL.heightDefault = leftEye;
    eyeR.heightDefault = rightEye;
}

// Set border radius for left and right eye
void RoboEyes::setBorderradius(byte leftEye, byte rightEye) {
    eyeL.borderRadiusNext = leftEye;
    eyeR.borderRadiusNext = rightEye;
    eyeL.borderRadiusDefault = leftEye;
    eyeR.borderRadiusDefault = rightEye;
}

// Set space between the eyes, can also be negative
void RoboEyes::setSpacebetween(int space) {
    spaceBetweenNext = space;
    spaceBetweenDefault = space;
}

// Set mood expression
void RoboEyes::setMood(unsigned char mood) {
    switch (mood) {
        case MOOD_TIRED:
            tired = 1;
            angry = 0;
            happy = 0;
            break;
        case MOOD_ANGRY:
            tired = 0;
            angry = 1;
            happy = 0;
            break;
        case MOOD_HAPPY:
            tired = 0;
            angry = 0;
            happy = 1;
            break;
        default:
            tired = 0;
            angry = 0;
            happy = 0;
            break;
    }
}

// Set predefined position
void RoboEyes::setPosition(unsigned char position) {
    switch (position) {
        case N:
            // North, top center
            eyeL.xNext = getScreenConstraint_X() / 2;
            eyeL.yNext = 0;
            break;
        case NE:
            // North-east, top right
            eyeL.xNext = getScreenConstraint_X();
            eyeL.yNext = 0;
            break;
        case E:
            // East, middle right
            eyeL.xNext = getScreenConstraint_X();
            eyeL.yNext = getScreenConstraint_Y() / 2;
            break;
        case SE:
            // South-east, bottom right
            eyeL.xNext = getScreenConstraint_X();
            eyeL.yNext = getScreenConstraint_Y();
            break;
        case S:
            // South, bottom center
            eyeL.xNext = getScreenConstraint_X() / 2;
            eyeL.yNext = getScreenConstraint_Y();
            break;
        case SW:
            // South-west, bottom left
            eyeL.xNext = 0;
            eyeL.yNext = getScreenConstraint_Y();
            break;
        case W:
            // West, middle left
            eyeL.xNext = 0;
            eyeL.yNext = getScreenConstraint_Y() / 2;
            break;
        case NW:
            // North-west, top left
            eyeL.xNext = 0;
            eyeL.yNext = 0;
            break;
        default:
            // Middle center
            eyeL.xNext = getScreenConstraint_X() / 2;
            eyeL.yNext = getScreenConstraint_Y() / 2;
            break;
    }
}

// Set automated eye blinking, minimal blink interval in full seconds and blink interval variation range in full seconds
void RoboEyes::setAutoblinker(bool active, int interval, int variation) {
    autoblinker = active;
    blinkInterval = interval;
    blinkIntervalVariation = variation;
}
void RoboEyes::setAutoblinker(bool active) {
    autoblinker = active;
}

// Set idle mode - automated eye repositioning, minimal time interval in full seconds and time interval variation range in full seconds
void RoboEyes::setIdleMode(bool active, int interval, int variation) {
    idle = active;
    idleInterval = interval;
    idleIntervalVariation = variation;
}
void RoboEyes::setIdleMode(bool active) {
    idle = active;
}

// Set curious mode - the respectively outer eye gets larger when looking left or right
void RoboEyes::setCuriosity(bool curiousBit) {
    curious = curiousBit;
}

// Set cyclops mode - show only one eye
// void RoboEyes::setCyclops(bool cyclopsBit) {
//     cyclops = cyclopsBit;
// }

// Set horizontal flickering (displacing eyes left/right)
void RoboEyes::setHFlicker(bool flickerBit, byte Amplitude) {
    hFlicker = flickerBit;          // turn flicker on or off
    hFlickerAmplitude = Amplitude;  // define amplitude of flickering in pixels
}
void RoboEyes::setHFlicker(bool flickerBit) {
    hFlicker = flickerBit;  // turn flicker on or off
}

// Set vertical flickering (displacing eyes up/down)
void RoboEyes::setVFlicker(bool flickerBit, byte Amplitude) {
    vFlicker = flickerBit;          // turn flicker on or off
    vFlickerAmplitude = Amplitude;  // define amplitude of flickering in pixels
}
void RoboEyes::setVFlicker(bool flickerBit) {
    vFlicker = flickerBit;  // turn flicker on or off
}

//*********************************************************************************************
//  GETTERS METHODS
//*********************************************************************************************

// Returns the max x position for left eye
int RoboEyes::getScreenConstraint_X() {
    return screenWidth - eyeL.widthCurrent - spaceBetweenCurrent - eyeR.widthCurrent;
}

// Returns the max y position for left eye
int RoboEyes::getScreenConstraint_Y() {
    return screenHeight - eyeL.heightDefault;  // using default height here, because height will vary when blinking and in curious mode
}

//*********************************************************************************************
//  BASIC ANIMATION METHODS
//*********************************************************************************************

// BLINKING FOR BOTH EYES AT ONCE
// Close both eyes
void RoboEyes::close() {
    eyeL.heightNext = 1;  // closing left eye
    eyeR.heightNext = 1;  // closing right eye
    eyeL_open = 0;        // left eye not opened (=closed)
    eyeR_open = 0;        // right eye not opened (=closed)
}

// Open both eyes
void RoboEyes::open() {
    eyeL_open = 1;  // left eye opened - if true, drawEyes() will take care of opening eyes again
    eyeR_open = 1;  // right eye opened
}

// Trigger eyeblink animation
void RoboEyes::blink() {
    close();
    open();
}

// BLINKING FOR SINGLE EYES, CONTROL EACH EYE SEPARATELY
// Close eye(s)
void RoboEyes::close(bool left, bool right) {
    if (left) {
        eyeL.heightNext = 1;  // blinking left eye
        eyeL_open = 0;        // left eye not opened (=closed)
    }
    if (right) {
        eyeR.heightNext = 1;  // blinking right eye
        eyeR_open = 0;        // right eye not opened (=closed)
    }
}

// Open eye(s)
void RoboEyes::open(bool left, bool right) {
    if (left) {
        eyeL_open = 1;  // left eye opened - if true, drawEyes() will take care of opening eyes again
    }
    if (right) {
        eyeR_open = 1;  // right eye opened
    }
}

// Trigger eyeblink(s) animation
void RoboEyes::blink(bool left, bool right) {
    close(left, right);
    open(left, right);
}

//*********************************************************************************************
//  MACRO ANIMATION METHODS
//*********************************************************************************************

// Play confused animation - one shot animation of eyes shaking left and right
void RoboEyes::anim_confused() {
    confused = 1;
}

// Play laugh animation - one shot animation of eyes shaking up and down
void RoboEyes::anim_laugh() {
    laugh = 1;
}

//*********************************************************************************************
//  PRE-CALCULATIONS AND ACTUAL DRAWINGS
//*********************************************************************************************

void RoboEyes::apply_macro() {
    //// APPLYING MACRO ANIMATIONS ////

    if (autoblinker) {
        if (millis() >= blinktimer) {
            blink();
            blinktimer = millis() + (blinkInterval * 1000) + (random(blinkIntervalVariation) * 1000);  // calculate next time for blinking
        }
    }

    // Laughing - eyes shaking up and down for the duration defined by laughAnimationDuration (default = 500ms)
    if (laugh) {
        if (laughToggle) {
            setVFlicker(1, 5);
            laughAnimationTimer = millis();
            laughToggle = 0;
        } else if (millis() >= laughAnimationTimer + laughAnimationDuration) {
            setVFlicker(0, 0);
            laughToggle = 1;
            laugh = 0;
        }
    }

    // Confused - eyes shaking left and right for the duration defined by confusedAnimationDuration (default = 500ms)
    if (confused) {
        if (confusedToggle) {
            setHFlicker(1, 20);
            confusedAnimationTimer = millis();
            confusedToggle = 0;
        } else if (millis() >= confusedAnimationTimer + confusedAnimationDuration) {
            setHFlicker(0, 0);
            confusedToggle = 1;
            confused = 0;
        }
    }

    // Idle - eyes moving to random positions on screen
    if (idle) {
        if (millis() >= idleAnimationTimer) {
            eyeL.xNext = random(getScreenConstraint_X());
            eyeL.yNext = random(getScreenConstraint_Y());
            idleAnimationTimer = millis() + (idleInterval * 1000) + (random(idleIntervalVariation) * 1000);  // calculate next time for eyes repositioning
        }
    }

    // Adding offsets for horizontal flickering/shivering
    if (hFlicker) {
        if (hFlickerAlternate) {
            eyeL.x += hFlickerAmplitude;
            eyeR.x += hFlickerAmplitude;
        } else {
            eyeL.x -= hFlickerAmplitude;
            eyeR.x -= hFlickerAmplitude;
        }
        hFlickerAlternate = !hFlickerAlternate;
    }

    // Adding offsets for horizontal flickering/shivering
    if (vFlicker) {
        if (vFlickerAlternate) {
            eyeL.y += vFlickerAmplitude;
            eyeR.y += vFlickerAmplitude;
        } else {
            eyeL.y -= vFlickerAmplitude;
            eyeR.y -= vFlickerAmplitude;
        }
        vFlickerAlternate = !vFlickerAlternate;
    }
}

void RoboEyes::drawEyes() {
    //// PRE-CALCULATIONS - EYE SIZES AND VALUES FOR ANIMATION TWEENINGS ////

    // Vertical size offset for larger eyes when looking left or right (curious gaze)
    if (curious) {
        if (eyeL.xNext <= 10) {
            eyeL.heightOffset = 8;
            // } else if (eyeL.xNext >= (getScreenConstraint_X() - 10) && cyclops) {
            //     eyeL.heightOffset = 8;
        } else {
            eyeL.heightOffset = 0;
        }  // left eye

        if (eyeR.xNext >= screenWidth - eyeR.widthCurrent - 10) {
            eyeR.heightOffset = 8;
        } else {
            eyeR.heightOffset = 0;
        }  // right eye
    } else {
        eyeL.heightOffset = 0;  // reset height offset for left eye
        eyeR.heightOffset = 0;  // reset height offset for right eye
    }

    // Left eye height
    eyeL.heightCurrent = (eyeL.heightCurrent + eyeL.heightNext + eyeL.heightOffset) / 2;
    eyeL.y = ((screenHeight - eyeL.heightDefault) / 2) - eyeL.heightOffset;

    // Right eye height
    eyeR.heightCurrent = (eyeR.heightCurrent + eyeR.heightNext + eyeR.heightOffset) / 2;
    eyeR.y = ((screenHeight - eyeR.heightDefault) / 2) - eyeR.heightOffset;

    // Open eyes again after closing them
    if (eyeL_open) {
        if (eyeL.heightCurrent <= 1 + eyeL.heightOffset) {
            eyeL.heightNext = eyeL.heightDefault;
        }
    }
    if (eyeR_open) {
        if (eyeR.heightCurrent <= 1 + eyeR.heightOffset) {
            eyeR.heightNext = eyeR.heightDefault;
        }
    }

    // Left eye width
    eyeL.widthCurrent = (eyeL.widthCurrent + eyeL.widthNext) / 2;
    // Right eye width
    eyeR.widthCurrent = (eyeR.widthCurrent + eyeR.widthNext) / 2;

    // Space between eyes
    spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext) / 2;

    // Left eye coordinates
    eyeL.x = (eyeL.x + eyeL.xNext) / 2;
    eyeL.y = (eyeL.y + eyeL.yNext) / 2;
    // Right eye coordinates
    eyeR.xNext = eyeL.xNext + eyeL.widthCurrent + spaceBetweenCurrent;  // right eye's x position depends on left eyes position + the space between
    eyeR.yNext = eyeL.yNext;                                            // right eye's y position should be the same as for the left eye
    eyeR.x = (eyeR.x + eyeR.xNext) / 2;
    eyeR.y = (eyeR.y + eyeR.yNext) / 2;

    // Left eye border radius
    eyeL.borderRadiusCurrent = (eyeL.borderRadiusCurrent + eyeL.borderRadiusNext) / 2;
    // Right eye border radius
    eyeR.borderRadiusCurrent = (eyeR.borderRadiusCurrent + eyeR.borderRadiusNext) / 2;

    //// ACTUAL DRAWINGS ////

    display->clearDisplay();  // start with a blank screen

    // Draw basic eye rectangles
    display->fillRoundRect(eyeL.x, eyeL.y, eyeL.widthCurrent, eyeL.heightCurrent, eyeL.borderRadiusCurrent, MAINCOLOR);  // left eye
                                                                                                                         // if (!cyclops) {
    display->fillRoundRect(eyeR.x, eyeR.y, eyeR.widthCurrent, eyeR.heightCurrent, eyeR.borderRadiusCurrent, MAINCOLOR);  // right eye
    // }

    apply_macro();

    // Prepare mood type transitions

    if (tired) {
        eyelidsTiredHeightNext = eyeL.heightCurrent / 2;
        eyelidsAngryHeightNext = 0;
    } else {
        eyelidsTiredHeightNext = 0;
    }
    if (angry) {
        eyelidsAngryHeightNext = eyeL.heightCurrent / 2;
        eyelidsTiredHeightNext = 0;
    } else {
        eyelidsAngryHeightNext = 0;
    }
    if (happy) {
        eyelidsHappyBottomOffsetNext = eyeL.heightCurrent / 2;
    } else {
        eyelidsHappyBottomOffsetNext = 0;
    }

    // Draw tired top eyelids
    eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext) / 2;
    display->fillTriangle(eyeL.x, eyeL.y - 1, eyeL.x + eyeL.widthCurrent, eyeL.y - 1, eyeL.x, eyeL.y + eyelidsTiredHeight - 1, BGCOLOR);                      // left eye
    display->fillTriangle(eyeR.x, eyeR.y - 1, eyeR.x + eyeR.widthCurrent, eyeR.y - 1, eyeR.x + eyeR.widthCurrent, eyeR.y + eyelidsTiredHeight - 1, BGCOLOR);  // right eye

    // Draw angry top eyelids
    eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext) / 2;
    display->fillTriangle(eyeL.x, eyeL.y - 1, eyeL.x + eyeL.widthCurrent, eyeL.y - 1, eyeL.x + eyeL.widthCurrent, eyeL.y + eyelidsAngryHeight - 1, BGCOLOR);  // left eye
    display->fillTriangle(eyeR.x, eyeR.y - 1, eyeR.x + eyeR.widthCurrent, eyeR.y - 1, eyeR.x, eyeR.y + eyelidsAngryHeight - 1, BGCOLOR);                      // right eye

    eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext) / 2;
    display->fillRoundRect(eyeL.x - 1, (eyeL.y + eyeL.heightCurrent) - eyelidsHappyBottomOffset + 1, eyeL.widthCurrent + 2, eyeL.heightDefault, eyeL.borderRadiusCurrent, BGCOLOR);  // left eye
    display->fillRoundRect(eyeR.x - 1, (eyeR.y + eyeR.heightCurrent) - eyelidsHappyBottomOffset + 1, eyeR.widthCurrent + 2, eyeR.heightDefault, eyeR.borderRadiusCurrent, BGCOLOR);  // right eye

    display->display();  // show drawings on display

}  // end of drawEyes method