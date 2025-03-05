void beginDisplay()
{
  Wire.begin();
  //Wire.begin(21, 22); //SDA, SCL

  Wire.setClock(1000000);

  if (display.begin() == false)
  {
    Serial.println("Display not detected");
    return;
  }
  Serial.println("Display Online");
  displayOnline = true;
  display.erase();

  //Turn on a pixel to start
  display.pixel(display.getWidth() / 2, display.getHeight() / 2);
  display.display();
}

void beginAccel()
{
  if (accelerometer.beginI2C() != BMA400_OK)
    Serial.println("Accelerometer not detected");
  else
  {
    Serial.println("Accelerometer connected!");
    accelerometerOnline = true;
  }
}

void beginEncoders()
{
  pinMode(encoderX_PinA, INPUT_PULLUP);
  pinMode(encoderX_PinB, INPUT_PULLUP);
  pinMode(encoderY_PinA, INPUT_PULLUP);
  pinMode(encoderY_PinB, INPUT_PULLUP);

  //Call updateEncoder() when any high/low changed seen on the encoder pins
  attachInterrupt(encoderX_PinA, updateEncoderX, CHANGE);
  attachInterrupt(encoderX_PinB, updateEncoderX, CHANGE);
  attachInterrupt(encoderY_PinA, updateEncoderY, CHANGE);
  attachInterrupt(encoderY_PinB, updateEncoderY, CHANGE);

  if (displayOnline)
  {
//    encoderCountX = display.getWidth() / 2;
//    encoderCountY = display.getHeight() / 2;
    encoderCountX = display.getWidth(); //64pps hack
    encoderCountY = display.getHeight();
  }
}
