#include <MSAMSComponents.h>
#include <oled.h>

/*
*  DISCLAIMER
*  When MSAMSComponents library is included
*  we have to define one pointer for each
*  LED, Button or Potentiometer of their
*  base class MSAMSComponent so that we are
*  able to configure each one (invoke void cfg()).
*  Example:
*  //create Button component
*  Button b(6);
*  
*  //create a base (MSAMSComponent) pointer
*  MSAMSComponent *bs = &b;
*  //in void setup(), configure b as:
*  bs->cfg();
*/

bool cleared = false;
const int colSpan = 32;
int bs, ps[4];
char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
String channel[5] = {"", "", "", ""};
Button btn(6);
Potentiometer p[4] = {A0, A1, A2, A3};
OLED display(
  4,
  2,
  NO_RESET_PIN,
  OLED::W_128,
  OLED::H_64,
  OLED::CTRL_SSD1306,
  0x3c
);

void charStr(int num, OLED &display_, int x = 0, int y = 0) {
  String sn = String(num);
  const int size_ = (sizeof sn / sizeof num) + 1;
  char getNum[size_];
  for(int i = 0; i <= size_; i++) {
    if(i == size_) getNum[i] = '\0';
    else getNum[i] = sn[i];
  }
  const char num_ = getNum;
  display_.draw_string(x, y, num_);
}

void charLine(int num, OLED &display_, int x = 0, int y = 0) {
  int mn = map(num, 0, 1023, OLED::H_64 - 1, 0);
  display_.draw_line(x, mn, x, y);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ;
  }
  display.begin();
  MSAMSComponent *bc = &btn;
  bc->cfg();
  for(int i = 0; i < 4; i++) {
    MSAMSComponent *pc = p + i;
    pc->cfg();
  }
}

void loop() {
  for(int i = 0; i < 4; i++) {
    ps[i] = p[i].getState();
    channel[i] = (String)ps[i];
    switch(i) {
      case 0:
        channel[i] = a + channel[i];
        break;
      case 1:
        channel[i] = b + channel[i];
        break;
      case 2:
        channel[i] = c + channel[i];
        break;
      case 3:
        channel[i] = d + channel[i];
        break;
    }
  }
  bs = btn.getState();
  channel[4] = (String)bs;
  channel[4] = e + channel[4];
  if(bs == 1) {
    display.clear();
    for(int i = 0; i < 4; i++) {
      int x;
      x = (i + 1) + (colSpan * i) + (colSpan / 2);
      charLine(ps[i], display, x, OLED::H_64 - 1);
    }
    display.display();
    cleared = false;
  }
  else {
    if(!cleared) {
      display.clear();
      display.display();
      cleared = true;
      delay(100);
    }
    if(Serial) {
      for(int i = 0; i < 4; i++) {
        Serial.println(channel[i]);
      }
      Serial.println(channel[4]);
    }
    delay(50);
  }
}
