float d = 3;
float px[] = { -d, d, d, -d, -d, d, d, -d};
float py[] = { -d, -d, d, d, -d, -d, d, d};
float pz[] = { -d, -d, -d, -d, d, d, d, d};

float p2x[8] = {0};
float p2y[8] = {0};
float r[3] = {0};

#define ROTATION_SPEED 00

void drawCube()
{
  //  r[0] = r[0] + PI / 180.0; // Add a degree
  //  r[1] = r[1] + PI / 180.0; // Add a degree

  r[0] = encoderCountX * (PI / 180.0);
  r[1] = encoderCountY * (PI / 180.0);

  //Serial.printf("r0: %0.3f r1: %0.3f\r\n", r[0], r[1]);

  //r[2] = r[2] + PI / 180.0; // Add a degree

  if (r[0] >= 360.0 * PI / 180.0)
  {
    r[0] = 0;
    encoderCountX = 0;
  }
  
  if (r[1] >= 360.0 * PI / 180.0)
  {
    r[1] = 0;
    encoderCountY = 0;
  }
  
  if (r[2] >= 360.0 * PI / 180.0)
    r[2] = 0;

  // This routine gets called often, so just make these statics
  static float px2, py2, pz2, px3, py3, pz3, ax, ay, az;

  for (int i = 0; i < 8; i++)
  {
    px2 = px[i];
    py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
    pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

    px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
    py3 = py2;
    pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

    ax = cos(r[2]) * px3 - sin(r[2]) * py3;
    ay = sin(r[2]) * px3 + cos(r[2]) * py3;
    az = pz3 - 150;

    p2x[i] = display.getWidth() / 2 + ax * cubeSize / az;
    p2y[i] = display.getHeight() / 2 + ay * cubeSize / az;
  }

  // Calculate draw time
  uint32_t startTime = millis();

  display.erase();
  for (int i = 0; i < 3; i++)
  {
    display.line(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1]);
    display.line(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5]);
    display.line(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4]);
  }

  display.line(p2x[3], p2y[3], p2x[0], p2y[0]);
  display.line(p2x[7], p2y[7], p2x[4], p2y[4]);
  display.line(p2x[3], p2y[3], p2x[7], p2y[7]);
  display.display();
}

void screenSaver()
{
  r[0] = r[0] + PI / 180.0; // Add a degree
  r[1] = r[1] + PI / 180.0; // Add a degree
  r[2] = r[2] + PI / 180.0; // Add a degree

  if (r[0] >= 360.0 * PI / 180.0)
  {
    r[0] = 0;
    encoderCountX = 0;
  }
  
  if (r[1] >= 360.0 * PI / 180.0)
  {
    r[1] = 0;
    encoderCountY = 0;
  }
  
  if (r[2] >= 360.0 * PI / 180.0)
    r[2] = 0;

  // This routine gets called often, so just make these statics
  static float px2, py2, pz2, px3, py3, pz3, ax, ay, az;

  for (int i = 0; i < 8; i++)
  {
    px2 = px[i];
    py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
    pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

    px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
    py3 = py2;
    pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

    ax = cos(r[2]) * px3 - sin(r[2]) * py3;
    ay = sin(r[2]) * px3 + cos(r[2]) * py3;
    az = pz3 - 150;

    p2x[i] = display.getWidth() / 2 + ax * cubeSize / az;
    p2y[i] = display.getHeight() / 2 + ay * cubeSize / az;
  }

  // Calculate draw time
  uint32_t startTime = millis();

  display.erase();
  for (int i = 0; i < 3; i++)
  {
    display.line(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1]);
    display.line(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5]);
    display.line(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4]);
  }

  display.line(p2x[3], p2y[3], p2x[0], p2y[0]);
  display.line(p2x[7], p2y[7], p2x[4], p2y[4]);
  display.line(p2x[3], p2y[3], p2x[7], p2y[7]);
  display.display();
}
