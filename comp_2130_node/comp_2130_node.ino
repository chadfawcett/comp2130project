#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

//  IO pins for Transceiver
#define CE_PIN   9
#define CSN_PIN 10

//  Define the transmit pipe
const uint64_t pipe = 0xE8E8F0F0E1LL;

//  Create a Radio and Servo
RF24 radio(CE_PIN, CSN_PIN);
Servo myservo;

char message[6];

void setup()
{
  delay(1000);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  myservo.attach(6);
  myservo.write(180);  //  Open vent by default
}

void loop()
{ 
  if ( radio.available() )
  {
    //  Continue reading from radio until nothing else is available to read
    bool done = false;
    while (!done)
    {
      // Get message
      done = radio.read(message, sizeof(message));
      if (strcmp(message, "open") == 0) {
        myservo.write(180);
      } else if (strcmp(message, "close") == 0) {
        myservo.write(10);
      }
    }
  }
}

