Adafruit_GFX_Button screen_main_buttons[8];
char screen_main_labels[8][7] = {"LEFT", "RIGHT", "TEETH", "ANGLE", "0", "0", "MANUAL", "START"};

void screen_main_setup()
{
  int px = 10;
  int py = 15;
  int w = tft.width() / 2 - px;
  int h = 45;
  int x = px;
  int y = 10;

  // Left
  screen_main_buttons[0].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, screen_main_labels[0], 2);

  x += w;

  // Right
  screen_main_buttons[1].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, screen_main_labels[1], 2);

  x = px;
  y += h + py;

  // Teeth checkbox
  screen_main_buttons[2].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, screen_main_labels[2], 2);

  x += w;
  
  // Angle checkbox
  screen_main_buttons[3].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, screen_main_labels[3], 2);

  x = px;
  y += h + py;
  
  // Teeth value
  screen_main_buttons[4].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_BLACK, COLOR_WHITE, screen_main_labels[4], 2);

  x += w;
  
  // Angle value
  screen_main_buttons[5].initButtonUL(&tft, x, y, w, h, COLOR_BLACK, COLOR_BLACK, COLOR_WHITE, screen_main_labels[5], 2);

  x = px;
  y += h + py;

  // Manual
  screen_main_buttons[6].initButtonUL(&tft, x, y, w * 2, h + 8, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_main_labels[6], 3);

  x = px;
  y += h + py + 8;
  
  // Start
  screen_main_buttons[7].initButtonUL(&tft, x, y, w * 2, h + 8, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_main_labels[7], 3);
}

void screen_main_enter()
{
  digitalWrite(IO_DIR, dir == DIR_LEFT ? LOW : HIGH);
  
  tft.fillScreen(COLOR_BLACK);
  screen_main_left_draw();
  screen_main_right_draw();
  screen_main_teeth_checkbox_draw();
  screen_main_angle_checkbox_draw();
  screen_main_teeth_value_draw();
  screen_main_angle_value_draw();
  screen_main_manual_draw();
  screen_main_start_draw();
}

void screen_main_leave()
{
  
}

void screen_main_loop()
{
  TSPoint p = getPoint();

  if (p.x == -1 || p.y == -1) return;

  if (screen_main_left_handle(p)) return;
  if (screen_main_right_handle(p)) return;
  if (screen_main_teeth_checkbox_handle(p)) return;
  if (screen_main_angle_checkbox_handle(p)) return;
  if (screen_main_teeth_value_handle(p)) return;
  if (screen_main_angle_value_handle(p)) return;
  if (screen_main_manual_handle(p)) return;
  if (screen_main_start_handle(p)) return;
  
  delay(100);
}

void screen_main_left_draw()
{
  if (dir == DIR_LEFT)
  {
    screen_main_buttons[0].setColor(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  }
  else
  {
    screen_main_buttons[0].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_main_buttons[0].drawButton(false);
}

bool screen_main_left_handle(TSPoint p)
{
  if (!screen_main_buttons[0].contains(p.x, p.y)) return false;

  if (dir == DIR_LEFT) return true;

  dir = DIR_LEFT;

  digitalWrite(IO_DIR, LOW);

  screen_main_right_draw();
  screen_main_left_draw();

  return true;
}

void screen_main_right_draw()
{
  if (dir == DIR_RIGHT)
  {
    screen_main_buttons[1].setColor(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  }
  else
  {
    screen_main_buttons[1].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_main_buttons[1].drawButton(false);
}

bool screen_main_right_handle(TSPoint p)
{
  if (!screen_main_buttons[1].contains(p.x, p.y)) return false;

  if (dir == DIR_RIGHT) return true;

  dir = DIR_RIGHT;

  digitalWrite(IO_DIR, HIGH);

  screen_main_left_draw();
  screen_main_right_draw();

  return true;
}

void screen_main_teeth_checkbox_draw()
{
  if (mode == MODE_TEETH)
  {
    screen_main_buttons[2].setColor(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  }
  else
  {
    screen_main_buttons[2].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_main_buttons[2].drawButton(false);
}

bool screen_main_teeth_checkbox_handle(TSPoint p)
{
  if (!screen_main_buttons[2].contains(p.x, p.y)) return false;

  if (mode == MODE_TEETH) return true;

  mode = MODE_TEETH;

  screen_main_angle_checkbox_draw();
  screen_main_teeth_checkbox_draw();
  screen_main_angle_value_draw();
  screen_main_teeth_value_draw();

  return true;
}

void screen_main_angle_checkbox_draw()
{
  if (mode == MODE_ANGLE)
  {
    screen_main_buttons[3].setColor(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  }
  else
  {
    screen_main_buttons[3].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_main_buttons[3].drawButton(false);
}

bool screen_main_angle_checkbox_handle(TSPoint p)
{
  if (!screen_main_buttons[3].contains(p.x, p.y)) return false;

  if (mode == MODE_ANGLE) return true;

  mode = MODE_ANGLE;

  screen_main_teeth_checkbox_draw();
  screen_main_angle_checkbox_draw();
  screen_main_teeth_value_draw();
  screen_main_angle_value_draw();

  return true;
}

void screen_main_teeth_value_draw()
{
  if (mode == MODE_TEETH)
  {
    screen_main_buttons[4].setColor(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
  }
  else
  {
    screen_main_buttons[4].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }

  screen_main_buttons[4].setLabel((char *)String(teethValue).c_str());
  screen_main_buttons[4].drawButton(false);
}

bool screen_main_teeth_value_handle(TSPoint p)
{
  if (!screen_main_buttons[4].contains(p.x, p.y)) return false;

  mode = MODE_TEETH;

  change_screen(SCREEN_KEYBOARD);

  return true;
}

void screen_main_angle_value_draw()
{
  if (mode == MODE_ANGLE)
  {
    screen_main_buttons[5].setColor(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
  }
  else
  {
    screen_main_buttons[5].setColor(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  }

  screen_main_buttons[5].setLabel((char *)String(angleValue, 3).c_str());
  screen_main_buttons[5].drawButton(false);
}

bool screen_main_angle_value_handle(TSPoint p)
{
  if (!screen_main_buttons[5].contains(p.x, p.y)) return false;

  mode = MODE_ANGLE;
  
  change_screen(SCREEN_KEYBOARD);

  return true;
}

void screen_main_manual_draw()
{
  screen_main_buttons[6].drawButton(false);
}

bool screen_main_manual_handle(TSPoint p)
{
  if (!screen_main_buttons[6].contains(p.x, p.y)) return false;

  change_screen(SCREEN_MANUAL);

  return true;
}

void screen_main_start_draw()
{
  screen_main_buttons[7].drawButton(false);
}

bool screen_main_start_handle(TSPoint p)
{
  if (!screen_main_buttons[7].contains(p.x, p.y)) return false;

  change_screen(SCREEN_RUN);

  return true;
}
