#define KEYBOARD_PADDING 10

Adafruit_GFX_Button screen_keyboard_buttons[14];
char screen_keyboard_labels[14][4] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "CLR", "DEL", "SET"};
String screen_keyboard_value;

void screen_keyboard_setup()
{
  byte w = (tft.width() - 4 * KEYBOARD_PADDING) / 3;
  byte h = (tft.height() - 7 * KEYBOARD_PADDING) / 6;
  int y = KEYBOARD_PADDING + h + KEYBOARD_PADDING;

  // CLR
  screen_keyboard_buttons[11].initButtonUL(&tft, KEYBOARD_PADDING * 2 + w, y, w, h, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_keyboard_labels[11], 2);
  
  // DEL
  screen_keyboard_buttons[12].initButtonUL(&tft, KEYBOARD_PADDING * 3 + w * 2, y, w, h, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_keyboard_labels[12], 2);

  for (byte r = 0; r < 3; ++r)
  {
    y += KEYBOARD_PADDING + h;
    
    for (byte c = 0; c < 3; ++c)
    {
      byte i = 1 + r * 3 + c;
      int x = KEYBOARD_PADDING + KEYBOARD_PADDING * c + w * c;
      
      screen_keyboard_buttons[i].initButtonUL(&tft, x, y, w, h, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_keyboard_labels[i], 2);
    }
  }

  y += KEYBOARD_PADDING + h;
  
  // Comma
  screen_keyboard_buttons[10].initButtonUL(&tft, KEYBOARD_PADDING, y, w, h, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_keyboard_labels[10], 2);
  
  // Zero
  screen_keyboard_buttons[0].initButtonUL(&tft, KEYBOARD_PADDING * 2 + w, y, w, h, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, screen_keyboard_labels[0], 2);
  
  // SET
  screen_keyboard_buttons[13].initButtonUL(&tft, KEYBOARD_PADDING * 3 + w * 2, y, w, h, COLOR_WHITE, COLOR_BLUE, COLOR_WHITE, screen_keyboard_labels[13], 2);
}

void screen_keyboard_enter()
{
  tft.fillScreen(COLOR_BLACK);

  if (mode == MODE_TEETH)
  {
    screen_keyboard_value = String(teethValue);
  }
  else
  {
    screen_keyboard_value = String(angleValue, 3);
  }

  screen_keyboard_value_draw(false);

  screen_keyboard_buttons[11].drawButton(false);
  screen_keyboard_buttons[12].drawButton(false);

  for (byte i = 1; i <= 9; ++i)
  {
    screen_keyboard_buttons[i].drawButton(false);
  }

  if (mode == MODE_ANGLE)
  {
    screen_keyboard_buttons[10].drawButton(false);
  }
  
  screen_keyboard_buttons[0].drawButton(false);
  screen_keyboard_buttons[13].drawButton(false);
}

void screen_keyboard_leave()
{
  
}

void screen_keyboard_loop()
{
  TSPoint p = getPoint();

  for (byte i = 0; i < 14; ++i)
  {
    if (screen_keyboard_buttons[i].contains(p.x, p.y))
    {
      screen_keyboard_buttons[i].press(true);
    }
    else
    {
      screen_keyboard_buttons[i].press(false);
    }
  }

  if (screen_keyboard_buttons[13].justReleased())
  {
    screen_keyboard_set();

    return;
  }

  if (screen_keyboard_buttons[10].justReleased())
  {
    screen_keyboard_comma();

    return;
  }
  
  if (screen_keyboard_buttons[11].justReleased())
  {
    screen_keyboard_clr();

    return;
  }
  
  if (screen_keyboard_buttons[12].justReleased())
  {
    screen_keyboard_del();

    return;
  }

  for (byte i = 0; i <= 9; ++i)
  {
    if (screen_keyboard_buttons[i].justReleased())
    {
      screen_keyboard_n(i);

      return;
    }
  }
}

void screen_keyboard_value_draw(bool invalid)
{
  tft.fillRect(KEYBOARD_PADDING, KEYBOARD_PADDING, tft.width() - KEYBOARD_PADDING * 2, (tft.height() - 7 * KEYBOARD_PADDING) / 6, invalid ? COLOR_RED : COLOR_WHITE);
  tft.setCursor(KEYBOARD_PADDING + 1 + 10, KEYBOARD_PADDING + 1 + ((tft.height() - 7 * KEYBOARD_PADDING) / 6) / 2 - 12);
  tft.setTextColor(invalid ? COLOR_WHITE : COLOR_BLACK);
  tft.setTextSize(3);
  tft.print(screen_keyboard_value);
}

void screen_keyboard_value_error()
{
  screen_keyboard_value_draw(true);
  delay(333);
  screen_keyboard_value_draw(false);
  delay(333);
}

void screen_keyboard_set()
{
  if (mode == MODE_TEETH)
  {
    teethValue = screen_keyboard_value.toInt();

    if (teethValue < 1)
    {
      teethValue = 1;
    }
    else if (teethValue > 360)
    {
      teethValue = 360;
    }

    angleValue = 360.0 / teethValue;
  }
  else
  {
    angleValue = screen_keyboard_value.toFloat();

    if (angleValue < 1.0)
    {
      angleValue = 1.0;
    }
    else if (angleValue > 360.0)
    {
      angleValue = 360.0;
    }

    teethValue = round(360 / angleValue);
  }
  
  change_screen(SCREEN_MAIN);
}

void screen_keyboard_clr()
{
  unsigned int l = screen_keyboard_value.length();

  if (l == 0)
  {
    return;
  }
  
  screen_keyboard_value.remove(0, l);
  screen_keyboard_value_draw(false);
}

void screen_keyboard_del()
{
  unsigned int l = screen_keyboard_value.length();

  if (l == 0)
  {
    return;
  }
  
  screen_keyboard_value.remove(l - 1, 1);
  screen_keyboard_value_draw(false);
  delay(200);
}

void screen_keyboard_comma()
{
  if (mode != MODE_ANGLE)
  {
    return;
  }

  char comma = '.';
  int commaI = screen_keyboard_value.indexOf(comma);
  
  if (commaI != -1)
  {
    screen_keyboard_value_error();
    
    return;
  }

  if (screen_keyboard_value.length() == 0)
  {
    screen_keyboard_value.concat("0.");
  }
  else
  {
    screen_keyboard_value.concat(comma);
  }
  
  screen_keyboard_value_draw(false);
  delay(200);
}

void screen_keyboard_n(byte n)
{
  unsigned int l = screen_keyboard_value.length();
  
  if (l == 7)
  {
    screen_keyboard_value_error();
    
    return;
  }

  if (l == 1 && screen_keyboard_value[0] == '0')
  {
    screen_keyboard_value.remove(0, 1);
  }

  screen_keyboard_value.concat(n);
  screen_keyboard_value_draw(false);
  delay(200);
}
