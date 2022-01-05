#include "DrawingBoard.h"
#include "M5Core2.h"
#include "lvgl.h"

#define DISP_HOR_RES 320
#define DISP_VER_RES 240

TFT_eSPI tft = TFT_eSPI(DISP_HOR_RES, DISP_VER_RES);
DrawingBoard drawing_board = DrawingBoard(tft);

static lv_disp_draw_buf_t draw_buf;

void tft_lv_initialization() {
  lv_init();

  static lv_color_t buf1[(DISP_HOR_RES * DISP_VER_RES) /
                         10]; /*Declare a buffer for 1/10 screen size*/
  static lv_color_t
      buf2[(DISP_HOR_RES * DISP_VER_RES) / 10];  // second buffer is optionnal

  lv_disp_draw_buf_init(
      &draw_buf, buf1, buf2,
      (DISP_HOR_RES * DISP_VER_RES) /
          10);  // Initialize `disp_buf` display buffer with the buffer(s). With
                // only one buffer use NULL instead buf_2

  tft.begin();         // TFT init
  tft.setRotation(1);  // 1=horizontal 3=upside down 0=left 2=right
}

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void init_disp_driver() {
  // Initialize the display
  static lv_disp_drv_t
      disp_drv; /*Descriptor of a display driver / //disp_drv.full_refresh =
                   true;  direct mode can also be set true*/

  lv_disp_drv_init(&disp_drv);       /*Basic initialization*/
  disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
  disp_drv.draw_buf = &draw_buf;     /*Assign the buffer to the display*/
  disp_drv.hor_res =
      DISP_HOR_RES; /*Set the horizontal resolution of the display*/
  disp_drv.ver_res =
      DISP_VER_RES; /*Set the vertical resolution of the display*/
  lv_disp_drv_register(&disp_drv); /*Finally register the driver*/
}

void setup() { tft_lv_initialization(); }

void loop() {
  lv_task_handler();

  drawing_board.loop();
}
