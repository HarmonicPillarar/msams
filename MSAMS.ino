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
const int rowSpan = 16;
int bs, p1s, p2s, p3s, p4s;
String channelA = "", channelB = "", channelC = "", channelD = "", channelE = "";
Button b(6);
Potentiometer p1(A0);
Potentiometer p2(A1);
Potentiometer p3(A2);
Potentiometer p4(A3);
OLED display(
  18,
  19,
  NO_RESET_PIN,
  OLED::W_128,
  OLED::H_64,
  OLED::CTRL_SSD1306,
  0x3c
);

void setup() {
  Serial.begin(115200);
  display.begin();
  MSAMSComponent *bc = &b;
  MSAMSComponent *p1c = &p1;
  MSAMSComponent *p2c = &p2;
  MSAMSComponent *p3c = &p3;
  MSAMSComponent *p4c = &p4;
  bc->cfg();
  p1c->cfg();
  p2c->cfg();
  p3c->cfg();
  p4c->cfg();
}

void charStr(int num, OLED &display_, int x = 0, int y = 0) {
  String sn = String(num);
  const int size_ = (sizeof sn / sizeof num) + 1;
  char getNum[size_];
  for(int i = 0; i <= size_; i++) {
    if(i == size_) getNum[i] = '\0';
    else getNum[i] = sn[i];
  }
  display_.draw_string(x, y, getNum);
}

void charLine(int num, OLED &display_, int x = 0, int y = 0) {
  int mn = map(num, 0, 1023, x, OLED::W_128);
  display_.draw_line(x, y, mn, y);
}

void loop() {
  p1s = p1.getState();
  channelA = (String)p1s;
  channelA = 'a' + channelA;
  p2s = p2.getState();
  channelB = (String)p2s;
  channelB = 'b' + channelB;
  p3s = p3.getState();
  channelC = (String)p3s;
  channelC = 'c' + channelC;
  p4s = p4.getState();
  channelD = (String)p4s;
  channelD = 'd' + channelD;
  bs = b.getState();
  channelE = (String)bs;
  channelE = 'e' + channelE;
  
  if(bs == 1) {
    charLine(p1s, display, 0, 0 + (rowSpan * 0));
    //charStr(p1s, display, 0, 1 + (rowSpan * 0));
    charLine(p2s, display, 0, 2 + (rowSpan * 1));
    //charStr(p2s, display, 0, 3 + (rowSpan * 1));
    charLine(p3s, display, 0, 4 + (rowSpan * 2));
    //charStr(p3s, display, 0, 5 + (rowSpan * 2));
    charLine(p4s, display, 0, 6 + (rowSpan * 3));
    //charStr(p4s, display, 0, 7 + (rowSpan * 3));
    display.display();
    display.clear();
    cleared = false;
  }
  else {
    if(!cleared) {
      display.clear();
      display.display();
      cleared = true;
      delay(100);
    }
    Serial.println(channelA);
    Serial.println(channelB);
    Serial.println(channelC);
    Serial.println(channelD);
    Serial.println(channelE);
    delay(50);
  }
}
