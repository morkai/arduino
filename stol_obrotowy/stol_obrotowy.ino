#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

#define ROTATION 0
#define MAX_PULSES 80000
#define PULSE_DURATION 5
#define PULSE_INTERVAL 800
#define IO_BUTTON 18
#define IO_DIR 19
#define IO_PULSE 20
#define IO_ENABLE 21
#define IO_ENABLE_ON LOW
#define IO_ENABLE_OFF HIGH

#define SCREEN_MAIN 1
#define SCREEN_KEYBOARD 2
#define SCREEN_RUN 3
#define SCREEN_MANUAL 4
byte screen = 0;

#define DIR_LEFT 0
#define DIR_RIGHT 1
byte dir = DIR_LEFT;

#define MODE_TEETH 0
#define MODE_ANGLE 1
byte mode = MODE_TEETH;

int teethValue = 100;
float angleValue = 360.0 / teethValue;

#define COLOR_BLACK       0x0000      /*   0,   0,   0 */
#define COLOR_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define COLOR_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define COLOR_BLUE        0x001F      /*   0,   0, 255 */
#define COLOR_GREEN       0x07E0      /*   0, 255,   0 */
#define COLOR_RED         0xF800      /* 255,   0,   0 */
#define COLOR_YELLOW      0xFFE0      /* 255, 255,   0 */
#define COLOR_WHITE       0xFFFF      /* 255, 255, 255 */
#define COLOR_ORANGE      0xFD20      /* 255, 165,   0 */

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 126
#define TS_MAXX 909
#define TS_MINY 88
#define TS_MAXY 916

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup()
{
  //Serial.begin(9600);

  pinMode(IO_BUTTON, INPUT_PULLUP);
  pinMode(IO_ENABLE, OUTPUT);
  pinMode(IO_PULSE, OUTPUT);
  pinMode(IO_DIR, OUTPUT);
  digitalWrite(IO_ENABLE, IO_ENABLE_ON);
  digitalWrite(IO_PULSE, HIGH);
  digitalWrite(IO_DIR, LOW);

  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(ROTATION);

  screen_main_setup();
  screen_keyboard_setup();
  screen_run_setup();
  screen_manual_setup();
  
  change_screen(SCREEN_MAIN);
}

void loop()
{
  loop_screen(screen);
}

void change_screen(byte newScreen)
{
  leave_screen(screen);
  enter_screen(newScreen);
  screen = newScreen;
  delay(500);
}

void leave_screen(byte screen)
{
  switch (screen)
  {
    case SCREEN_MAIN:
      screen_main_leave();
      break;

    case SCREEN_KEYBOARD:
      screen_keyboard_leave();
      break;

    case SCREEN_RUN:
      screen_run_leave();
      break;

    case SCREEN_MANUAL:
      screen_manual_leave();
      break;
  }
}

void enter_screen(byte screen)
{
  switch (screen)
  {
    case SCREEN_MAIN:
      screen_main_enter();
      break;

    case SCREEN_KEYBOARD:
      screen_keyboard_enter();
      break;

    case SCREEN_RUN:
      screen_run_enter();
      break;

    case SCREEN_MANUAL:
      screen_manual_enter();
      break;
  }
}

void loop_screen(byte screen)
{
  switch (screen)
  {
    case SCREEN_MAIN:
      screen_main_loop();
      break;

    case SCREEN_KEYBOARD:
      screen_keyboard_loop();
      break;

    case SCREEN_RUN:
      screen_run_loop();
      break;

    case SCREEN_MANUAL:
      screen_manual_loop();
      break;
  }
}

TSPoint getPoint()
{
  TSPoint p = ts.getPoint();

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z <= ts.pressureThreshhold)
  {
    p.x = p.y = p.z = -1;

    return p;
  }

  if (ROTATION == 0)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = tft.height() - map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  }
  else if (ROTATION == 1)
  {
    int x = p.x;
    int y = p.y;
    
    p.x = map(y, TS_MINY, TS_MAXY, 0, tft.width());
    p.y = map(x, TS_MINX, TS_MAXX, 0, tft.height());
  }
  else if (ROTATION == 2)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
  }
  else if (ROTATION == 3)
  {
    int x = p.x;
    int y = p.y;
    
    p.x = map(y, TS_MINY, TS_MAXY, tft.width(), 0);
    p.y = map(x, TS_MINX, TS_MAXX, tft.height(), 0);
  }

  return p;
}

void pulse()
{
  digitalWrite(IO_PULSE, LOW);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(IO_PULSE, HIGH);
  delayMicroseconds(PULSE_INTERVAL);
}
