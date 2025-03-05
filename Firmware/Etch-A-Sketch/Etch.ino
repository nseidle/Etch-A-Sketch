//Based on encoder values, draw pixels
void etchASketch()
{
  static int lastX = 0;
  static int lastY = 0;
  static int halfCount = 0;

  if (encoderChangeX == true || encoderChangeY == true)
  {
    encoderChangeX = false;
    encoderChangeY = false;

    if (encoderCountX < 0) encoderCountX = 0;
    if (encoderCountY < 0) encoderCountY = 0;

    //Cut count in two to emulate 64pps encoder

    if ((encoderCountX / 2) >= display.getWidth()) encoderCountX = (display.getWidth() * 2) - 1;
    if ((encoderCountY / 2) >= display.getHeight()) encoderCountY = (display.getHeight() * 2) - 1;

    if (displayOnline == true)
    {
      display.pixel(encoderCountX / 2, encoderCountY / 2);
      display.display();
    }

    //Turn off the pixel one step ahead of the head
    //Only calc header once every two clicks, 64pps emulation problem
    //if (++halfCount == 2)
    //{
    halfCount = 0;
    //Determine the direction the head is moving
    int x = encoderCountX;// / 2;
    int y = encoderCountY;// / 2;

    if (y > lastY)
    {
      Serial.println("Down");
      display.pixel((encoderCountX / 2), (encoderCountY / 2) + 1, 0);
    }
    else if (y < lastY)
    {
      Serial.println("Up");
      display.pixel((encoderCountX / 2), (encoderCountY / 2) - 1, 0);
    }

    if (x > lastX)
    {
      Serial.println("Right");
      display.pixel((encoderCountX / 2) + 1, (encoderCountY / 2), 0);
    }
    else if (x < lastX)
    {
      Serial.println("Left");
      display.pixel((encoderCountX / 2) - 1, (encoderCountY / 2), 0);
    }

    lastY = y;
    lastX = x;
    //}


    Serial.printf("pixel x: %d y: %d\r\n", encoderCountX / 2, encoderCountY / 2);
  }
}
