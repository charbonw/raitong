#include <String.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>
SoftwareSerial HC12(8, 9); // HC-12 TX Pin, HC-12 RX Pin
#define SETPIN 7 // HC_12 SET Pin

// init wired ethernet shield
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;

// Thingspeak info
// https://thingspeak.com/channels/568023
// username: charbonw@oregonstate.edu
// password: Raitong498
unsigned long myChannelNumber = 568023;
const char* myWriteAPIKey = "99L7B0W8SEV0TIOR";

// Serial read variables
const byte numChars = 64;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

void setup() {
  Ethernet.begin(mac);
  ThingSpeak.begin(client);
  
  setBaudRate();
  HC12.begin(1200);
  Serial.begin(9600);
}

// sometimes the LoRa module baud rate seems to reset from 1200 to 9600
// this ensures that it is set to 1200
// lower baud rates (1200 and 2400) will result in longer range
// I haven't tested this function
void setBaudRate() {
  // need to pull SET low before sending AT commands
  pinMode(SETPIN, OUTPUT);
  digitalWrite(SETPIN, LOW);
  HC12.begin(9600);
  HC12.print("AT+B1200");
  HC12.end();
  digitalWrite(SETPIN, HIGH);
}

void loop() {
  //testHC12();
  //testThingSpeak();
  recvWithEndMarker();
  showNewData();
}

void testThingSpeak() {
  Ethernet.begin(mac);
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, 10);
  ThingSpeak.setField(2, 9);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

void testHC12() {
  while (HC12.available()) {        // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
 
  while (HC12.available() > 0 && newData == false) {
    rc = HC12.read();
    Serial.write(rc);

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("Data Received: ");
    Serial.println(receivedChars);

    // data will be a comma-delimited string with 7 values
    // this splits the string into values and sets each to a ThingSpeak field
    char temp[24];
    int ndx = 0, count = 1;
    for (int i = 0; i < numChars; i++) {
      if (receivedChars[i] == '\0' || receivedChars[i] == '\r') {
        temp[ndx] = '\0';
        ThingSpeak.setField(count, temp);
        Serial.println(temp);
        break;
      }
      else if (receivedChars[i] == ',') {
        temp[ndx] = '\0';
        ThingSpeak.setField(count, temp);
        Serial.println(temp);
        count++;  
        ndx = 0;
      }
      else {
        temp[ndx] = receivedChars[i];
        ndx++;
      }
    }

    // pushes all fields
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(20000); // ThingSpeak will only accept updates every 15 seconds.
    newData = false;
  }
}

