#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <LiquidCrystal.h>

//  IO pins for Transceiver
#define CE_PIN   9
#define CSN_PIN 10

//  Define the transmit pipe
const uint64_t pipe = 0xE8E8F0F0E1LL;

//  Create a Radio
RF24 radio(CE_PIN, CSN_PIN);

//  Potentiometer pins
int optimalTempPin = 0;
int optimalTempVal = 0;

//  Temperature sensor pins
int tempPin = 1;
float tempVal = 0;

//  Define messages to send
//  Note: 'open' and 'close' are reserved words
char open_[6] = "open";
char close_[6] = "close";
char *prevMessage = open_;  //  Open by default

//  Create LCD Driver
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

//  Time stamp for updating LCD
unsigned long watch = 0;

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Starting...");
  
  radio.begin();
  radio.openWritingPipe(pipe);
}

void loop()
{
  //  Read user optimal temp input and map to appropriate room temperatures
  optimalTempVal = analogRead(optimalTempPin);
  optimalTempVal = map(optimalTempVal, 0, 1023, 15, 31);
  
  //  Read current temperature from sensor
  tempVal = analogRead(tempPin);
  //  Convert reading to degrees C
  tempVal = (tempVal * 5.0 / 1024.0 - 0.5) * 100;
  
  if (tempVal < optimalTempVal - 1) {
    radio.write(open_, sizeof(open_));
    prevMessage = open_;
  } else if(tempVal > optimalTempVal + 1) {
    radio.write(close_, sizeof(close_));
    prevMessage = close_;
  } else {
    radio.write(prevMessage, sizeof(prevMessage));
  }
  
  //  Arduino isn't powerful enought to update the lcd
  //  and send the command on each loop. This makes it
  //  so the LCD only updates one a second
  if (millis() - watch > 1000) {
    lcd.clear();
    lcd.print("Opt   Cur   Pos ");
    lcd.setCursor(0, 1);
    lcd.print(optimalTempVal);
    lcd.setCursor(6, 1);
    lcd.print(tempVal);
    lcd.setCursor(13, 1);
    lcd.print(strcmp(prevMessage, "open") == 0 ? "O" : "C");
    watch = millis();
  }
}

