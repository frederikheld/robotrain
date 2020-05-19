// -- import libraries

#include <U8g2lib.h>      // OLED display


// -- config

#define PIN_DISPLAY_SCL D1
#define PIN_DISPLAY_SDA D2


// -- define global objects

// OLED display:
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, PIN_DISPLAY_SCL, PIN_DISPLAY_SDA, U8X8_PIN_NONE);


// -- functions

void drawSpeedometer(int speed_actual_percent, int speed_nominal_percent) {

  uint8_t speed_actual_width = (int)(128.0 * ((float)speed_actual_percent / 100));
  uint8_t speed_nominal_pos = (int)(128.0 * ((float)speed_nominal_percent / 100));


  // actual speed as bar on top of display:
  u8g2.drawBox(0, 0, speed_actual_width, 16);

  // nominal speed as triangle below actual speed bar:
//  u8g2.drawTriangle(
//    speed_nominal_pos, 17,
//    speed_nominal_pos - 5, 23,
//    speed_nominal_pos + 5, 23
//  );

  // nominal speed as vertical XOR line
  // on top of the actual speed bar:
//  u8g2.setDrawColor(2);
//  u8g2.drawBox(
//    speed_nominal_pos, 0, 2, 16
//  );

  // nominal speed as two XOR triangles
  // on top of the actual speed bar:
  // (better readability at the edges compared t0 XOR bar)
  u8g2.setDrawColor(2);
  u8g2.drawTriangle(
    speed_nominal_pos, 5,
    speed_nominal_pos - 4, 0,
    speed_nominal_pos + 4, 0
  );
  u8g2.drawTriangle(
    speed_nominal_pos, 11,
    speed_nominal_pos - 4, 16,
    speed_nominal_pos + 4, 16
  );


}


// -- arduino functions

void setup() {

  // init serial:
  Serial.begin(19200);
  
  // init display:
  u8g2.begin();

  u8g2.setFont(u8g2_font_6x10_tf);

  // init analog number generator:
  randomSeed(analogRead(A0));
  
}

void loop() {
  int randomNumber = random(0, 100);
  
  for (int i = 0; i < 100; i++) {
    if (i % 10 == 0) {
      randomNumber = random(0, 100);
    }
    
    u8g2.clearBuffer();
    drawSpeedometer(i, randomNumber);
    u8g2.sendBuffer();
    
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(randomNumber);
    
    delay(10);
  }
  
  for (int i = 100; i > 0; i--) {
    if (i % 10 == 0) {
      randomNumber = random(0, 100);
    }
    
    u8g2.clearBuffer();
    drawSpeedometer(i, randomNumber);
    u8g2.sendBuffer();
    
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(randomNumber);
    
    delay(10);
  }
}
