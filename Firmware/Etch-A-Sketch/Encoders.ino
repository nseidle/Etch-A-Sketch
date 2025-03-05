//ISR for encoder - Called every time the encoder is twisted
//Based on my Qwiic Twist code: https://github.com/sparkfun/Qwiic_Twist/blob/master/Firmware/Qwiic_Twist/interrupts.ino
//Based on http://bildr.org/2012/08/rotary-encoder-arduino/
//Adam - I owe you many beers for bildr

//A complete tick occurs across four interrupts and looks like:
//ABAB.ABAB = 01001011 for CW
//ABAB.ABAB = 10000111 for CCW

//lastEncoded could be many things but by looking for two unique values
//we filter out corrupted and partially dropped encoder readings
//Guarantees we will not get partial indent readings

void IRAM_ATTR updateEncoderX()
{
  byte MSB = digitalRead(encoderX_PinA); //MSB = most significant bit
  byte LSB = digitalRead(encoderX_PinB); //LSB = least significant bit

  byte encoded = (MSB << 1) | LSB; //Convert the 2 pin value to single number
  lastEncodedX = (lastEncodedX << 2) | encoded; //Add this to the previous readings

  if (lastEncodedX == 0b01001011) //One tick clockwise
  {
    encoderCountX++;
    encoderChangeX = true;
  }
  else if (lastEncodedX == 0b10000111) //One tick counter clockwise
  {
    encoderCountX--;
    encoderChangeX = true;
  }

  lastEncoder = millis(); //Push out screen saver
  if (displayMode == DISPLAY_SCREENSAVER)
    exitScreenSaver = true;
}

void IRAM_ATTR updateEncoderY()
{
  byte MSB = digitalRead(encoderY_PinA); //MSB = most significant bit
  byte LSB = digitalRead(encoderY_PinB); //LSB = least significant bit

  byte encoded = (MSB << 1) | LSB; //Convert the 2 pin value to single number
  lastEncodedY = (lastEncodedY << 2) | encoded; //Add this to the previous readings

  if (lastEncodedY == 0b01001011) //One tick clockwise
  {
    encoderCountY++;
    encoderChangeY = true;
  }
  else if (lastEncodedY == 0b10000111) //One tick counter clockwise
  {
    encoderCountY--;
    encoderChangeY = true;
  }

  lastEncoder = millis(); //Push out screen saver
  if (displayMode == DISPLAY_SCREENSAVER)
    exitScreenSaver = true;
}
