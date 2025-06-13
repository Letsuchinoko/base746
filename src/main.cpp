#include "lvgl.h"
lv_obj_t* R;
lv_obj_t* G;
lv_obj_t* B;
lv_obj_t* Couleur;
lv_obj_t * sliderouge;
lv_obj_t * slidervert;
lv_obj_t * sliderbleu;
int rouge, vert, bleu,sr,sv,sb;
float red,green,blu;
uint16_t r, g, b, c, colorTemp, lux;
static void slider_event_cb_r(lv_event_t * e);
static void slider_event_cb_v(lv_event_t * e);
static void slider_event_cb_b(lv_event_t * e);
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
      LV_LOG_USER("Clicked");
    }
}

void testLvgl()
{
  // Initialisations générales
  lv_obj_t * label;
  R=lv_label_create(lv_screen_active());
  G=lv_label_create(lv_screen_active());
  B=lv_label_create(lv_screen_active());
  Couleur=lv_label_create(lv_screen_active());
  sliderouge= lv_slider_create(lv_screen_active());
  slidervert= lv_slider_create(lv_screen_active());
  sliderbleu= lv_slider_create(lv_screen_active());
  lv_obj_align(sliderouge, LV_ALIGN_CENTER,0,-90);
  lv_obj_align(slidervert, LV_ALIGN_CENTER,0,-60);
  lv_obj_align(sliderbleu,LV_ALIGN_CENTER,0,-30);
  lv_obj_align(R, LV_ALIGN_CENTER,0,10);
  lv_obj_align(G, LV_ALIGN_CENTER,0,30);
  lv_obj_align(B, LV_ALIGN_CENTER,0,50);
  lv_obj_align(Couleur, LV_ALIGN_CENTER,0,70);
  lv_slider_set_range(sliderouge, 0, 255);
  lv_slider_set_range(slidervert, 0, 255);
  lv_slider_set_range(sliderbleu, 0, 255);
  lv_label_set_text(R, "Rouge: 0");
  lv_label_set_text(G, "Vert: 0");
  lv_label_set_text(B, "Bleu: 0");
  lv_label_set_text(Couleur, "Couleur");
  lv_obj_t * btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(sliderouge, slider_event_cb_r, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(slidervert, slider_event_cb_v, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(sliderbleu, slider_event_cb_b, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 100);
  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "Comparer couleur");
  lv_obj_center(label);
}
#ifdef ARDUINO

#include "lvglDrivers.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"
#define SCL 32;
#define SDA 31;
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
// à décommenter pour tester la démo
// #include "demos/lv_demos.h"

void mySetup()
{
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  testLvgl();
}

void loop()
{
  lv_color_t col;
  lv_obj_t * scr = lv_scr_act(); 
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  tcs.getRGB(&red,&green,&blu);
  rouge=(unsigned int)red;
  vert=(unsigned int)green;
  bleu=(unsigned int)blu;
  sr=(int)lv_slider_get_value(sliderouge);
  sv=(int)lv_slider_get_value(slidervert);
  sb=(int)lv_slider_get_value(sliderbleu);
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(rouge, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(vert, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(bleu, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  if (r>500&&g>500&&b>500)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(255, 255, 255),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else if (r<100&&g<100&&b<100)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(255, 255, 255),LV_PART_MAIN);
  }
  
  else if (g>300&r<300&b<400)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(0, 255, 0),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else if(b>300&r<300&g<300)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(0, 0, 255),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else if(r>200&&g<200&b<200)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(255, 0, 0),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else if(rouge>=108&&rouge<=120)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(255, 128, 0),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else if(rouge==75&&vert>=100&&vert<=104)
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(255, 255, 0),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(0, 0, 0),LV_PART_MAIN);
  }
  else
  {
    lv_obj_set_style_bg_color(scr,col=lv_color_make(147, 21, 50),LV_PART_MAIN);
    lv_obj_set_style_text_color(scr,col=lv_color_make(255, 255, 255),LV_PART_MAIN);
  }
  if(sr==rouge&&sv==vert&&sb==bleu)
      {
        lv_label_set_text(Couleur, "Meme couleur");
      }
}

void myTask(void *pvParameters)
{
  // Init
  TickType_t xLastWakeTime;
  // Lecture du nombre de ticks quand la tâche débute
  xLastWakeTime = xTaskGetTickCount();
  while (1)
  {
    // Loop

    // Endort la tâche pendant le temps restant par rapport au réveil,
    // ici 200ms, donc la tâche s'effectue toutes les 200ms
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200)); // toutes les 200 ms
  }
}
static void slider_event_cb_r(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target_obj(e);
    char buf[20];
    lv_snprintf(buf, sizeof(buf), "Rouge: %d", (int)lv_slider_get_value(slider));
    lv_label_set_text(R, buf);
}
static void slider_event_cb_v(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target_obj(e);
    char buf[20];
    lv_snprintf(buf, sizeof(buf), "Vert: %d", (int)lv_slider_get_value(slider));
    lv_label_set_text(G, buf);
}
static void slider_event_cb_b(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target_obj(e);
    char buf[20];
    lv_snprintf(buf, sizeof(buf), "Bleu: %d", (int)lv_slider_get_value(slider));
    lv_label_set_text(B, buf);
}
#else

#include "lvgl.h"
#include "app_hal.h"
#include <cstdio>

int main(void)
{
  printf("LVGL Simulator\n");
  fflush(stdout);

  lv_init();
  hal_setup();

  testLvgl();

  hal_loop();
  return 0;
}

#endif
