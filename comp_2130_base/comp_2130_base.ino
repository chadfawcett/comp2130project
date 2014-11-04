/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  char message1[] = "on";
  for (int i = 0; i < 100; i++) {
    radio.write( message1, sizeof(message1) );
    delay(10);
  }

  char message2[] = "off";
  for (int i = 0; i < 100; i++) {
    radio.write( message2, sizeof(message2) );
    delay(10);
  }
}//--(end main loop )---

