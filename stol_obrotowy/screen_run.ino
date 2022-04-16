int screen_run_teeth;
float screen_run_angle;
int screen_run_steps_todo;
int screen_run_steps_done;
bool screen_run_running = false;
bool screen_run_io_pressed = false;
unsigned long screen_run_pulse_time;
unsigned int screen_run_pulse_remaining;
Adafruit_GFX_Button screen_run_back_button;
Adafruit_GFX_Button screen_run_toggle_button;
char screen_run_toggle_labels[][9] = {"RUN", "CONTINUE", "STOP", "DONE"};

void screen_run_setup()
{
  screen_run_toggle_button.initButtonUL(&tft, 15, 125, 240 - 30, 105, COLOR_ORANGE, COLOR_ORANGE, COLOR_BLACK, screen_run_toggle_labels[0], 3);
  screen_run_back_button.initButtonUL(&tft, 15, 320 - 15 - 60, 240 - 30, 60, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, "BACK", 2);
}

void screen_run_enter()
{
  if (mode == MODE_TEETH)
  {
    screen_run_teeth = teethValue;
    screen_run_angle = 360.0 / teethValue;
    screen_run_steps_todo = screen_run_teeth;
  }
  else
  {
    screen_run_angle = angleValue;
    screen_run_teeth = 360 / angleValue;
    screen_run_steps_todo = 1;
  }
  
  screen_run_steps_done = 0;
  
  tft.fillScreen(COLOR_BLACK);
  
  screen_run_settings_draw();
  screen_run_steps_draw();
  screen_run_toggle_draw();
  screen_run_back_draw();
}

void screen_run_leave()
{
  
}

void screen_run_loop()
{
  if (digitalRead(IO_BUTTON) == LOW)
  {
    screen_run_io_pressed = true;

    return;
  }
  
  if (screen_run_io_pressed)
  {
    screen_run_io_pressed = false;
    screen_run_toggle_handle();

    return;
  }
  
  TSPoint p = getPoint();

  if (p.x == -1 || p.y == -1)
  {
    if (screen_run_running)
    {
      screen_run_pulse();
    }
    
    return;
  }

  if (screen_run_back_handle(p)) return;
  if (screen_run_toggle_handle(p)) return;
}

void screen_run_settings_draw()
{
  String value;
  
  tft.setTextColor(COLOR_WHITE, COLOR_BLACK);
  tft.setTextSize(2);
  
  tft.setCursor(15, 15);
  tft.print("DIR:    ");
  value = dir == DIR_LEFT ? "LEFT " : "RIGHT";
  tft.println(value);
  
  tft.moveCursor(15, 5);
  tft.print("ENABLE: ");
  value = digitalRead(IO_ENABLE) == IO_ENABLE_ON ? "ON " : "OFF";
  tft.println(value);
  
  tft.moveCursor(15, 5);
  tft.print("TEETH:  ");
  value = String(screen_run_teeth);
  while (value.length() < 7) value.concat(' ');
  tft.println(value);
  
  tft.moveCursor(15, 5);
  tft.print("ANGLE:  ");
  value = String(screen_run_angle, 3);
  while (value.length() < 7) value.concat(' ');
  tft.println(value);
}

void screen_run_steps_draw()
{
  String value;
  
  tft.setTextColor(COLOR_WHITE, COLOR_BLACK);
  tft.setTextSize(2);
  tft.setCursor(15, 15 + (5 + 4 * 3) * 4 + 4 * 4);
  tft.print("STEPS:  ");
  value.concat(screen_run_steps_done);
  value.concat('/');
  value.concat(screen_run_steps_todo);
  while (value.length() < 7) value.concat(' ');
  tft.println(value);
}

void screen_run_back_draw()
{
  if (screen_run_running)
  {
    screen_run_back_button.setColor(COLOR_DARKGREY, COLOR_BLACK, COLOR_DARKGREY);
  }
  else
  {
    screen_run_back_button.setColor(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
  }
  
  screen_run_back_button.drawButton(false);
}

bool screen_run_back_handle(TSPoint p)
{
  if (!screen_run_back_button.contains(p.x, p.y)) return false;

  if (!screen_run_running)
  {
    change_screen(SCREEN_MAIN);
  }

  return true;
}

void screen_run_toggle_draw()
{
  // Stop
  if (screen_run_running)
  {
    screen_run_toggle_button.setColor(COLOR_RED, COLOR_RED, COLOR_WHITE);
    screen_run_toggle_button.setLabel(screen_run_toggle_labels[2]);
  }
  // Done
  else if (screen_run_steps_done >= screen_run_steps_todo)
  {
    screen_run_toggle_button.setColor(COLOR_GREEN, COLOR_GREEN, COLOR_WHITE);
    screen_run_toggle_button.setLabel(screen_run_toggle_labels[3]);
  }
  // Continue
  else if (screen_run_steps_done > 0)
  {
    screen_run_toggle_button.setColor(COLOR_ORANGE, COLOR_ORANGE, COLOR_BLACK);
    screen_run_toggle_button.setLabel(screen_run_toggle_labels[1]);
  }
  // Run
  else
  {
    screen_run_toggle_button.setColor(COLOR_ORANGE, COLOR_ORANGE, COLOR_BLACK);
    screen_run_toggle_button.setLabel(screen_run_toggle_labels[0]);
  }
  
  screen_run_toggle_button.drawButton(false);
}

bool screen_run_toggle_handle(TSPoint p)
{
  if (!screen_run_toggle_button.contains(p.x, p.y)) return false;

  return screen_run_toggle_handle();
}

bool screen_run_toggle_handle()
{
  // Stop
  if (screen_run_running)
  {
    screen_run_stop();
  }
  // Done
  else if (screen_run_steps_done >= screen_run_steps_todo)
  {
    change_screen(SCREEN_MAIN);

    return true;
  }
  // Continue
  else if (screen_run_steps_done > 0)
  {
    screen_run_continue();
  }
  // Run
  else
  {
    screen_run_run();
  }

  screen_run_toggle_draw();
  screen_run_back_draw();

  return true;
}

void screen_run_stop()
{
  screen_run_running = false;
}

void screen_run_run()
{
  screen_run_continue();
}

void screen_run_continue()
{
  screen_run_pulse_remaining = round(screen_run_angle * MAX_PULSES / 360);
  screen_run_running = true;
}

void screen_run_pulse()
{
  pulse();
  
  screen_run_pulse_remaining -= 1;

  if (screen_run_pulse_remaining > 0)
  {
    return;
  }

  screen_run_steps_done += 1;
  screen_run_running = false;

  screen_run_steps_draw();
  screen_run_toggle_draw();
  screen_run_back_draw();
}
