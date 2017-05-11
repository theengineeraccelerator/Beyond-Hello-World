/***************************************************
  Author: Malcolm Knapp
  Date: 4/11/17
  Description:
  This is adapted form the FONA FONA_SMS_Response and OLED Wing Examples by Adafruit
  The

  FONA Source
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*

  For use with FONA 800 & 808, not 3G
*/

#include "Adafruit_FONA.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7

// this is a large buffer for replies
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void virticalPrint (char* displayString) {
  int x = 8;
  int y = 0;
  int y0 = 0;
  int stringSize = strlen(displayString);
  Serial.println(stringSize);
  y0 = (4 - stringSize / 2);
  Serial.print("y "); Serial.println(y);
  char *dptrTo = displayString;
  char printChar;
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  for (y; y < stringSize; y++) {
    display.setCursor(10, (y0 + y) * 16);
    printChar = *dptrTo;
    Serial.print("Char "); Serial.println(printChar);
    display.print(printChar);
    *dptrTo++;
  }
  display.display();

}
void setup() {
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));
  // init display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Serial.println("OLED begun");
  // displya Adafruit Splash screen`
  display.display();
  delay(1000);
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  // set text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(1);
  virticalPrint("1");
  display.display();
  delay(1000);
  virticalPrint("12");
  display.display();
  delay(1000);
  virticalPrint("123");
  display.display();
  delay(1000);
  virticalPrint("1234");
  display.display();
  delay(1000);
  virticalPrint("12345");
  display.display();
  delay(1000);
  virticalPrint("123456");
  display.display();
  delay(1000);
  virticalPrint("1234567");
  display.display();
  delay(1000);
  virticalPrint("2 factor");
  display.display();
}

void loop () {}
