#pragma once

class DrawingBoard {
 public:
  DrawingBoard();
  void setup();
  void clear();
  void loop();

 protected:
  int8_t _size;
  int8_t _color;

  lv_obj_t* _color_label;
  lv_obj_t* _size_label;
  lv_obj_t* _clear_label;

  int32_t getColor();
  void changeColor();
  void changeSize();
  void drawPoint(int32_t x, int32_t y);

  void drawPoints();
  void drawPoint();
};
