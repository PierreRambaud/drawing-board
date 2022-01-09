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

static lv_disp_draw_buf_t draw_buf;
DrawingBoard *drawing_board;
M5Display *tft;

void tft_lv_initialization() {
  M5.begin();

  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // Declare a buffer for 1/10 screen siz
  static lv_color_t buf2[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];  // second buffer is optionnal

  // Initialize `disp_buf` display buffer with the buffer(s).
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10);

  tft = &M5.Lcd;
}

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft->startWrite();
  tft->setAddrWindow(area->x1, area->y1, w, h);
  tft->pushColors((uint16_t *)&color_p->full, w * h, true);
  tft->endWrite();

  lv_disp_flush_ready(disp);
}

void init_disp_driver() {
  // Initialize the display
  static lv_disp_drv_t disp_drv;  // Descriptor of a display driver

  lv_disp_drv_init(&disp_drv);  // Basic initialization

  disp_drv.flush_cb = my_disp_flush;  // Set your driver function
  disp_drv.draw_buf = &draw_buf;      // Assign the buffer to the display
  disp_drv.hor_res = LV_HOR_RES_MAX;  // Set the horizontal resolution of the display
  disp_drv.ver_res = LV_VER_RES_MAX;  // Set the vertical resolution of the display

  lv_disp_drv_register(&disp_drv);                   // Finally register the driver
  lv_disp_set_bg_color(NULL, lv_color_hex3(0x000));  // Set default background color to black
}

void setup() {
  tft_lv_initialization();

  init_disp_driver();

  // Setup Drawing board
  drawing_board = new DrawingBoard();
  drawing_board->setup();
}

void loop() {
  M5.update();

  lv_task_handler();

  drawing_board->loop();
}
