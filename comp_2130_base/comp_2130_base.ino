#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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

void setup()
{
  radio.begin();
  radio.openWritingPipe(pipe);
  
  Serial.begin(9600);
}

void loop()
{
  //  Read user optimal temp input and map to appropriate room temperatures
  optimalTempVal = analogRead(optimalTempPin);
  optimalTempVal = map(optimalTempVal, 0, 1023, 15, 31);
  Serial.print(optimalTempVal);
  Serial.print(" ");
  
  //  Read current temperature from sensor
  tempVal = analogRead(tempPin);
  //  Convert reading to degrees C
  tempVal = (tempVal * 5.0 / 1024.0 - 0.5) * 100;
  Serial.print(tempVal);
  Serial.print(" ");
  
  if (tempVal < optimalTempVal - 1) {
    radio.write(open_, sizeof(open_));
    prevMessage = open_;
  } else if(tempVal > optimalTempVal + 1) {
    radio.write(close_, sizeof(close_));
    prevMessage = close_;
  } else {
    radio.write(prevMessage, sizeof(prevMessage));
  }
  
  Serial.println(prevMessage);
}

