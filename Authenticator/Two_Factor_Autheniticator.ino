/***************************************************
  Author: Malcolm Knapp
  Date: 4/11/17
  Description:
  This is adapted form the FONA FONA_SMS_Response and OLED Wing Examples by Adafruit
  The 2-Factor Authenticator provide a side channel to recive SMS code messages 
  and displays them for the user. After the code is entered the display can be 
  reset.  

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
#define BUTTON_B 6

// this is a large buffer for replies
char replybuffer[255];

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void verticalPrint (char* displayString) {
  int x = 10;
  int offset = 16; // line size.
  int y = 0;
  int yOffset = 0; // line offset so that the text appears in the middle of the screen
  int stringSize = strlen(displayString);
  yOffset = (4 - stringSize / 2); // determine line offset based on srtring size
  char *dptrTo = displayString;
  char printChar;
  display.clearDisplay(); // Clear the buffer.
  display.display(); 
  for (y; y < stringSize; y++) { 
    display.setCursor(x, (yOffset + y) * 16); //go to the next line
    printChar = *dptrTo;  // print that character
    display.print(printChar);
    *dptrTo++; 
  }
  display.setCursor(0, 0);
  display.display();

}

void setup() {
  //while (!Serial);
  delay(1000);
  Serial.begin(115200);
  pinMode(BUTTON_B, INPUT_PULLUP);

  // init display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Serial.println("OLED begun");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(3);
  display.display();// display Adafruit Splash screen`
  delay(1000);
  display.clearDisplay(); // Clear the buffer.
  display.display(); // diplay buffer
  verticalPrint("2 factor");

  // init FONA
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println("FONA Ready");
}

char fonaInBuffer[64];          //for notifications from the FONA

void loop() {

  char* bufPtr = fonaInBuffer;    //handy buffer pointer

  if (fona.available())      //any data available from the FONA?
  {
    int slot = 0;            //this will be the slot number of the SMS
    int charCount = 0;
    uint16_t smslen;
    //Read the notification into fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaInBuffer) - 1)));

    //Add a terminal NULL to the notification string
    *bufPtr = 0;

    //Scan the notification string for an SMS received notification.
    //  If it's an SMS message, we'll get the slot number in 'slot'
    if (1 == sscanf(fonaInBuffer, "+CMTI: \"SM\",%d", &slot)) {
      if (! fona.getSMSSender(slot, replybuffer, 250)) { // get sender number
        Serial.println("Failed!");
      }
      Serial.print(F("FROM: ")); Serial.println(replybuffer);

      // Retrieve SMS value.
      uint16_t smslen;
      if (! fona.readSMS(slot, replybuffer, 250, &smslen)) { // pass in buffer and max len!
        Serial.println("Failed!");
      }
      Serial.print(F("SMS # ")); Serial.print(slot);
      Serial.print(F(" of length ")); Serial.print(smslen);
      Serial.print(F(" with message: ")); Serial.println(replybuffer);
      verticalPrint(replybuffer); // display code

      // delete the original msg after it is processed
      if (fona.deleteSMS(slot)) {
        Serial.println(F("OK!"));
      } else {
        Serial.println(F("Couldn't delete"));
      }
    }
  }

  if (! digitalRead(BUTTON_B))
  {
    verticalPrint("2 factor");
  }
}
