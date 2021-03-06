#include <M5Core2.h>
#include <lvgl.h>

#include "DrawingBoard.h"

DrawingBoard::DrawingBoard() {
  _size = 5;
  _color = 0;
}

static lv_style_t style_color_label;
static lv_style_t style_size_label;
static lv_style_t style_clear_label;

void DrawingBoard::setup() {
  lv_style_set_text_color(&style_color_label, lv_color_hex3(getColor()));
  lv_style_set_text_color(&style_size_label, lv_color_white());
  lv_style_set_text_color(&style_clear_label, lv_color_white());

  _color_label = lv_label_create(lv_scr_act());
  lv_label_set_text(_color_label, "Color");
  lv_obj_set_pos(_color_label, 50, 220);
  lv_obj_add_style(_color_label, &style_color_label, 0);

  _size_label = lv_label_create(lv_scr_act());
  lv_label_set_text_fmt(_size_label, "Size: %d", _size);
  lv_obj_set_pos(_size_label, 145, 220);
  lv_obj_add_style(_size_label, &style_size_label, 0);

  _clear_label = lv_label_create(lv_scr_act());
  lv_label_set_text(_clear_label, "Clear");
  lv_obj_set_pos(_clear_label, 235, 220);
  lv_obj_add_style(_clear_label, &style_clear_label, 0);

  clear();
}

int32_t DrawingBoard::getColor() {
  switch (_color) {
    case 1:
      return GREEN;
    case 2:
      return BLUE;
    case 3:
      return RED;
    case 4:
      return YELLOW;
    case 5:
      return PINK;
    case 6:
      return CYAN;
    case 7:
      return ORANGE;
    default:
      return WHITE;
  }
}

void DrawingBoard::drawPoint(int32_t x, int32_t y) {
  M5.Lcd.drawCircle(x, y, _size, getColor());
  M5.Lcd.fillCircle(x, y, _size, getColor());

  Serial.printf("Draw point: %d, %d\r\n", x, y);
}

void DrawingBoard::changeColor() {
  _color = _color + 1;
  if (_color >= 8) {
    _color = 0;
  }

  lv_style_set_text_color(&style_color_label, lv_color_hex3(getColor()));
  lv_obj_report_style_change(&style_color_label);

  Serial.printf("Color changed: %d\r\n", _color);
}

void DrawingBoard::changeSize() {
  _size = _size + 5;
  if (_size == 20) {
    _size = 5;
  }

  lv_label_set_text_fmt(_size_label, "Size: %d", _size);
  Serial.printf("Size changed: %d\r\n", _size);
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
  M5.Lcd.fillScreen(TFT_BLACK);
  lv_obj_report_style_change(&style_color_label);
  lv_obj_report_style_change(&style_clear_label);
  lv_label_set_text_fmt(_size_label, "Size: %d", _size);
}

void DrawingBoard::loop() {
  if (M5.BtnA.wasPressed()) {
    changeColor();
  } else if (M5.BtnB.wasPressed()) {
    changeSize();
  } else if (M5.BtnC.wasPressed()) {
    clear();
  }

  drawPoints();
}
