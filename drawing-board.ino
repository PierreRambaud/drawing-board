/**
If not configured in your lv_conf.h, please uncomment

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240
#define LV_TICK_CUSTOM 1
#define LV_COLOR_DEPTH 16
*/

#include "M5Core2.h"
#include "lvgl.h"
#include "DrawingBoard.h"

/* TFT_eSPI tft = TFT_eSPI(LV_HOR_RES_MAX, LV_VER_RES_MAX); */
M5Display& tft = M5.Lcd;
DrawingBoard drawing_board = DrawingBoard(tft);

static lv_disp_draw_buf_t draw_buf;

void tft_lv_initialization() {
  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10]; /*Declare a buffer for 1/10 screen size*/
  static lv_color_t buf2[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10]; // second buffer is optionnal

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10); // Initialize `disp_buf` display buffer with the buffer(s). With only one buffer use NULL instead buf_2

  M5.begin();
  tft.setRotation(1); // 1=horizontal 3=upside down 0=left 2=right
}

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*) &color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void init_disp_driver() {
  // Initialize the display
  static lv_disp_drv_t disp_drv;     /*Descriptor of a display driver / //disp_drv.full_refresh = true;  direct mode can also be set true*/

  lv_disp_drv_init(&disp_drv);       /*Basic initialization*/

  disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
  disp_drv.draw_buf = &draw_buf;     /*Assign the buffer to the display*/
  disp_drv.hor_res = LV_HOR_RES_MAX;   /*Set the horizontal resolution of the display*/
  disp_drv.ver_res = LV_VER_RES_MAX;   /*Set the vertical resolution of the display*/

  lv_disp_drv_register(&disp_drv);   /*Finally register the driver*/
  lv_disp_set_bg_color(NULL, lv_color_hex3(0x000));
}


void setup() {
  tft_lv_initialization();

  init_disp_driver();

  drawing_board.setup();
  /* drawing_board.clear(); */
}

void loop() {
  M5.update();

  lv_task_handler();

  drawing_board.loop();
}
