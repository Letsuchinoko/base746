#include "lvgl.h"
lv_obj_t* temp;
lv_obj_t* imperator;
lv_obj_t* R;
lv_obj_t* G;
lv_obj_t* B;
lv_obj_t* C;
lv_obj_t* Couleur;
int temperature, lumiere, rouge, vert, bleu, contraste;
float red,green,blu;
uint16_t r, g, b, c, colorTemp, lux;
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
      LV_LOG_USER("Clicked");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void testLvgl()
{
  // Initialisations générales
  lv_obj_t * label;

  temp=lv_label_create(lv_screen_active());
  imperator=lv_label_create(lv_screen_active());
  R=lv_label_create(lv_screen_active());
  G=lv_label_create(lv_screen_active());
  B=lv_label_create(lv_screen_active());
  C=lv_label_create(lv_screen_active());
  Couleur=lv_label_create(lv_screen_active());
  lv_obj_align(temp, LV_ALIGN_CENTER,0,-100);
  lv_obj_align(imperator, LV_ALIGN_CENTER,0,-80);
  lv_obj_align(R, LV_ALIGN_CENTER,0,-60);
  lv_obj_align(G, LV_ALIGN_CENTER,0,-40);
  lv_obj_align(B, LV_ALIGN_CENTER,0,-20);
  lv_obj_align(C, LV_ALIGN_CENTER,0,0);
  lv_obj_align(Couleur, LV_ALIGN_CENTER,0,20);
  lv_label_set_text(temp, "Temperature");
  lv_label_set_text(imperator, "Luminosite");
  lv_label_set_text(R, "Rouge");
  lv_label_set_text(G, "Vert");
  lv_label_set_text(B, "Bleu");
  lv_label_set_text(C, "0");
  lv_label_set_text(Couleur, "Couleur");
  lv_obj_t * btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 60);
  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "Bouton");
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
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  temperature=colorTemp;
  lumiere=lux;
  contraste=c;
  tcs.getRGB(&red,&green,&blu);
  rouge=(unsigned int)red;
  vert=(unsigned int)green;
  bleu=(unsigned int)blu;
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  lv_label_set_text_fmt(temp, "Temperature :%dK", temperature);
  lv_label_set_text_fmt(imperator,"Luminosite :%dlx", lumiere);
  lv_label_set_text_fmt(R, "Rouge :%d",rouge);
  lv_label_set_text_fmt(G, "Vert :%d",vert);
  lv_label_set_text_fmt(B, "Bleu :%d",bleu);
  lv_label_set_text_fmt(C, "%d",contraste);
  if (r>500&&g>500&&b>500)
  {
    lv_label_set_text(Couleur,"Blanc");
  }
  else if (r<100&&g<100&&b<100)
  {
    lv_label_set_text(Couleur, "Noir");
  }
  
  else if (g>300&r<300&b<400)
  {
    lv_label_set_text(Couleur,"Vert");
  }
  else if(b>300&r<300&g<300)
  {
    lv_label_set_text(Couleur,"Bleu");
  }
  else if(r>200&&g<200&b<200)
  {
    lv_label_set_text(Couleur, "Rouge");
  }
  else if(rouge>=108&&rouge<=111)
  {
    lv_label_set_text(Couleur,"Orange");
  }
  else if(rouge==75&&vert==100)
  {
    lv_label_set_text(Couleur,"Jaune");
  }
  else
  {
    lv_label_set_text(Couleur,"Couleur non specifie");
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
