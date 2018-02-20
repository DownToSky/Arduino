#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
#include <TouchScreen.h>
#include <stdint.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define LCD_RESET A4

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Elegoo_GFX_Button button;
/*
class Button{
  public:
  Elegoo_TFTLCD* tft;
  short x1, x2, y1, y2;
  void (*macro)();
  void set_macro(void (*new_macro)());
  void set_coord(short new_x1, short new_x2, short new_y1, short new_y2);
  Button(short new_x1, short new_x2, short new_y1, short new_y2, Elegoo_TFTLCD* new_tft);
};

Button::Button(short new_x1, short new_x2, short new_y1, short new_y2, Elegoo_TFTLCD* new_tft){
 tft = new_tft;
 set_coord(new_x1, new_x2, new_y1, new_y2);
 Serial.println("hi");
}

void Button::set_coord(short new_x1, short new_x2, short new_y1, short new_y2) {
  x1 = new_x1;
  x2 = new_x2;
  y1 = new_y1;
  y2 = new_y2;
 tft->fillRect(x1, y1, abs(x2-x1), abs(y2-y1), RED);
}
void Button::set_macro(void (*new_macro)()){
  macro = new_macro;
}*/

void setup() {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
  
  }
  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  button.initButton(&tft,320/2,240/2,100,100,RED,GREEN,BLUE,(char*)"",1);
  button.drawButton();
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

boolean state = false;

void loop() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    if(button.contains(p.x, p.y))
      if(state == false)
      {
        button.drawButton(true);
        state = true;
      }
   }
   else
      if(state == true)
      {
        
        button.drawButton();
        state = false;
      }
  Serial.print(state);
  Serial.print("\n");
  delay(100);
}
