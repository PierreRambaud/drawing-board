#include <M5Core2.h>
#include <lvgl.h>

#include "DrawingBoard.h"

lv_obj_t* drawing_label(char* title, int32_t x, int32_t y) {
  lv_obj_t* label = lv_label_create(lv_scr_act());
  lv_label_set_recolor(label, true);
  lv_label_set_text(label, title);
  lv_obj_set_x(label, x);
  lv_obj_set_y(label, y);

  return label;
}

char* writeSize(int32_t size) {
  char buf[8];
  sprintf(buf, "Size: %d", size);

  return buf;
}

DrawingBoard::DrawingBoard(TFT_eSPI tft) {
  _size = 5;
  _color = 0;

  _color_label = drawing_label("Color", 50, 220);
  _size_label = drawing_label(writeSize(_size), 145, 220);
  _clear_label = drawing_label("Clear", 235, 220);
}

int32_t DrawingBoard::getColor() {
  switch (_color) {
    case 1:
      return TFT_GREEN;
    case 2:
      return TFT_BLUE;
    case 3:
      return TFT_RED;
    case 4:
      return TFT_YELLOW;
    case 5:
      return TFT_PINK;
    case 6:
      return TFT_CYAN;
    case 7:
      return TFT_ORANGE;
    default:
      return TFT_WHITE;
  }
}

void DrawingBoard::drawPoint(int32_t x, int32_t y) {
  _tft->drawCircle(x, y, _size, getColor());
  _tft->fillCircle(x, y, _size, getColor());
}

void DrawingBoard::changeColor() {
  _color = _color + 1;
  if (_color >= 8) {
    _color = 0;
  }

  lv_obj_set_style_text_color(_color_label, lv_color_hex(getColor()), LV_STATE_ANY);
}

void DrawingBoard::changeSize() {
  _size = _size + 5;
  if (_size == 20) {
    _size = 5;
  }

  lv_label_set_text(_size_label, writeSize(_size));
}

void DrawingBoard::drawPoints() {
  if (M5.Touch.ispressed()) {
    TouchPoint_t coordinate = M5.Touch.getPressPoint();
    if (coordinate.y == -1 or coordinate.x == -1) {
      return;
    }

    if ((coordinate.y >= 220 - _size) == false) {
      drawPoint(coordinate.x, coordinate.y);
    }
  }
}

void DrawingBoard::clear() {
  _tft->fillScreen(TFT_BLACK);

  _color = 0;
  changeColor();
  _size = 5;
  changeSize();
}

void DrawingBoard::loop() {
  if (M5.BtnA.isPressed()) {
    changeColor();
  } else if (M5.BtnB.isPressed()) {
    changeSize();
  } else if (M5.BtnC.isPressed()) {
    clear();
  }

  drawPoints();

  delay(20);
}
