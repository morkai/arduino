unsigned long screen_manual_pressed_at = 0;
bool screen_manual_pulse = false;
Adafruit_GFX_Button screen_manual_buttons[4];
char screen_manual_labels[4][7] = {"LEFT", "ENABLE", "RIGHT", "BACK"};
int screen_manual_enable = LOW;

void screen_manual_setup()
{
  int p = 15;
  int x = p;
  int y = 10;
  int w = tft.width() - 2 * p;
  
  screen_manual_buttons[0].initButtonUL(&tft, x, y, w, 60, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_manual_labels[0], 2);

  y += 60 + p;
  
  screen_manual_buttons[1].initButtonUL(&tft, x, y, w, 75, COLOR_RED, COLOR_RED, COLOR_WHITE, screen_manual_labels[1], 3);

  y += 75 + p;
  
  screen_manual_buttons[2].initButtonUL(&tft, x, y, w, 60, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_manual_labels[2], 2);

  y += 60 + p;
  
  screen_manual_buttons[3].initButtonUL(&tft, x, y, w, 60, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_manual_labels[3], 2);
}

void screen_manual_enter()
{
  screen_manual_pressed_at = 0;
  screen_manual_pulse = false;
  screen_manual_enable = digitalRead(IO_ENABLE);
  
  tft.fillScreen(COLOR_BLACK);

  screen_manual_left_draw();
  screen_manual_enable_draw();
  screen_manual_right_draw();
  screen_manual_back_draw();
}

void screen_manual_leave()
{
  
}

void screen_manual_loop()
{
  if (digitalRead(IO_BUTTON) == LOW)
  {
    if (screen_manual_pulse)
    {
      pulse();
    }
    else if (screen_manual_pressed_at == 0)
    {
      screen_manual_pressed_at = millis();

      pulse();
    }
    else if (millis() - screen_manual_pressed_at > 1000)
    {
      screen_manual_pulse = true;

      pulse();
    }

    return;
  }
  else
  {
    screen_manual_pressed_at = 0;
    screen_manual_pulse = false;
  }
  
  TSPoint p = getPoint();

  if (p.x == -1 || p.y == -1) return;

  if (screen_manual_left_handle(p)) return;
  if (screen_manual_enable_handle(p)) return;
  if (screen_manual_right_handle(p)) return;
  if (screen_manual_back_handle(p)) return;
  
  delay(100);
}

void screen_manual_left_draw()
{
  if (digitalRead(IO_DIR) == LOW)
  {
    screen_manual_buttons[0].setColor(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  }
  else
  {
    screen_manual_buttons[0].setColor(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_manual_buttons[0].drawButton(false);
}

bool screen_manual_left_handle(TSPoint p)
{
  if (!screen_manual_buttons[0].contains(p.x, p.y)) return false;

  if (digitalRead(IO_DIR) == HIGH)
  {
    digitalWrite(IO_DIR, LOW);
    delayMicroseconds(10);
  
    screen_manual_right_draw();
    screen_manual_left_draw();
  }
  
  pulse();

  return true;
}

void screen_manual_enable_draw()
{
  if (screen_manual_enable == IO_ENABLE_ON)
  {
    screen_manual_buttons[1].setColor(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  }
  else
  {
    screen_manual_buttons[1].setColor(COLOR_RED, COLOR_RED, COLOR_WHITE);
  }
  
  screen_manual_buttons[1].drawButton(false);
}

bool screen_manual_enable_handle(TSPoint p)
{
  if (!screen_manual_buttons[1].contains(p.x, p.y)) return false;

  screen_manual_enable = screen_manual_enable == IO_ENABLE_ON ? IO_ENABLE_OFF : IO_ENABLE_ON;
  
  digitalWrite(IO_ENABLE, screen_manual_enable);

  screen_manual_enable_draw();

  return true;
}

void screen_manual_right_draw()
{
  if (digitalRead(IO_DIR) == HIGH)
  {
    screen_manual_buttons[2].setColor(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  }
  else
  {
    screen_manual_buttons[2].setColor(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_manual_buttons[2].drawButton(false);
}

bool screen_manual_right_handle(TSPoint p)
{
  if (!screen_manual_buttons[2].contains(p.x, p.y)) return false;

  if (digitalRead(IO_DIR) == LOW)
  {
    digitalWrite(IO_DIR, HIGH);
    delayMicroseconds(10);
  
    screen_manual_left_draw();
    screen_manual_right_draw();
  }
  
  pulse();

  return true;
}

void screen_manual_back_draw()
{
  screen_manual_buttons[3].drawButton(false);
}

bool screen_manual_back_handle(TSPoint p)
{
  if (!screen_manual_buttons[3].contains(p.x, p.y)) return false;

  change_screen(SCREEN_MAIN);

  return true;
}
