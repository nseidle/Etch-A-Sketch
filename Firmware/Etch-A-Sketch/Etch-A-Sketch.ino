/*
  Nathan Seidle
  January 7th, 2025

  Read two encoders for X/Y coordinates
  Display pixels on OLED
  Erase the display if shaken
*/

enum displayType
{
  DISPLAY_ETCHASKETCH = 0,
  DISPLAY_CUBE,
  DISPLAY_SCREENSAVER,
};

//Pins
const int encoderX_PinA = 25;
const int encoderX_PinB = 17;
const int encoderY_PinA = 26;
const int encoderY_PinB = 27;

#include <SparkFun_Qwiic_OLED.h> //http://librarymanager/All#SparkFun_Qwiic_OLED
Qwiic1in3OLED display;
bool displayOnline = false;

#include "SparkFun_BMA400_Arduino_Library.h"
BMA400 accelerometer;
bool accelerometerOnline = false;

volatile int16_t encoderCountX = 0; //+ for CW, - for CCW
volatile uint8_t lastEncodedX = 0; //Used to compare encoder readings between interrupts. Helps detect turn direction.
volatile bool encoderChangeX = false; // Allows for print when encoder has changed
volatile int16_t encoderCountY = 0; //+ for CW, - for CCW
volatile uint8_t lastEncodedY = 0; //Used to compare encoder readings between interrupts. Helps detect turn direction.
volatile bool encoderChangeY = false; // Allows for print when encoder has changed

extern void IRAM_ATTR updateEncoderX(); //It's over in the other INO, I promise...
extern void IRAM_ATTR updateEncoderY(); //It's over in the other INO, I promise...

int displayMode = 0; //Default to etchASketch
uint32_t lastEncoder = 0; //Timer to enter screen saver
volatile bool exitScreenSaver = false;

int cubeSize = 950;
bool cubeSizeIncrease = true;
uint32_t lastCubeSizeChange = 0;
uint32_t lastShake = 0;

void setup()
{
  Serial.begin(115200);
  delay(250);
  Serial.println("Etch-A-Sketch");

  beginDisplay();
  beginAccel();
  beginEncoders();
}

void loop()
{
  if (Serial.available())
  {
    byte incoming = Serial.read();
    if (incoming == 'r')
    {
      ESP.restart();
    }
    else if (incoming == 't')
    {
    }
    else if (incoming == 'd')
    {
    }
  }
  updateAccelerometer();
  updateDisplay();

  if (millis() - lastEncoder > 8000 && displayMode != 2)
  {
    Serial.println("Screen saver");
    displayMode = 2;
  }

  //  //Modify the size of the cube while in screen saver mode
  //  if(displayMode == DISPLAY_SCREENSAVER)
  //  {
  //    //Check if we need to change the cube size
  //    if(millis() - lastCubeSizeChange > 50)
  //    {
  //      lastCubeSizeChange = millis();
  //
  //      //See if we are making it bigger or smaller
  //      if(cubeSizeIncrease == true)
  //      {
  //        cubeSize++;
  //        if(cubeSize > 950)
  //        {
  //          cubeSizeIncrease = false;
  //        }
  //      }
  //      else if(cubeSizeIncrease == false)
  //      {
  //        cubeSize--;
  //        if(cubeSize < 200)
  //        {
  //          cubeSizeIncrease = true;
  //        }
  //      }
  //    }
  //  }

  if (exitScreenSaver == true)
  {
    exitScreenSaver = false;
    changeDisplayMode(DISPLAY_ETCHASKETCH);
  }
}

//Based on encoder values, draw pixels
void updateDisplay()
{
  if (displayMode == DISPLAY_ETCHASKETCH)
    etchASketch();
  else if (displayMode == DISPLAY_CUBE)
    drawCube();
  else if (displayMode == DISPLAY_SCREENSAVER)
    screenSaver();
}

void updateAccelerometer()
{
  if (accelerometerOnline == true)
  {
    // Get measurements from the sensor. This must be called before accessing
    // the acceleration data, otherwise it will never update
    accelerometer.getSensorData();

    double vector = 0;

    vector += (accelerometer.data.accelX * accelerometer.data.accelX);
    vector += (accelerometer.data.accelY * accelerometer.data.accelY);
    vector += (accelerometer.data.accelZ * accelerometer.data.accelZ);
    vector = sqrt(vector);

    if (abs(vector) > 2.5 && (millis() - lastShake > 1500))
    {
      lastShake = millis();

      Serial.println("Shake it!");

      if (displayMode == DISPLAY_ETCHASKETCH)
        changeDisplayMode(DISPLAY_CUBE);
      else if (displayMode == DISPLAY_CUBE)
        changeDisplayMode(DISPLAY_ETCHASKETCH);
      else if (displayMode == DISPLAY_SCREENSAVER)
        changeDisplayMode(DISPLAY_ETCHASKETCH);
    }
  }
}

// If encoder interrupt or accel shake cause display change, reset vars as need
void changeDisplayMode(uint8_t newMode)
{
  displayMode = newMode;

  if (displayOnline)
  {
    //Apply reset values
    display.erase();
    display.display();
    if (displayMode == DISPLAY_ETCHASKETCH)
    {
      encoderCountX = display.getWidth();
      encoderCountY = display.getHeight();
      display.pixel(encoderCountX / 2, encoderCountY / 2);//64pps hack
      display.display();
    }
  }
}
