//Largely based on Lewis Callaway's Instructable code:
// http://www.instructables.com/id/NFC-Computer-Unlocker

#include <Adafruit_PN532.h>
#include <Wire.h>
#include <SPI.h>

#define IRQ 2 // this trace must be cut and rewired to work on Leonardo
#define RESET 3

Adafruit_PN532 nfc(IRQ, RESET);

void setup() {
  Serial.begin(115200);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
 
  // configure board to read RFID tags
  nfc.SAMConfig();
  //Keyboard.begin(); //initiate the keyboard
}

unsigned digit = 0;

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // wait for RFID card to show up!
  Serial.println("Waiting for an ISO14443A Card ...");

  // Wait for an ISO14443A type cards (Mifare, etc.). When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  uint32_t cardidentifier = 0;
 
  if (success) {
    // Found a card!

    Serial.print("Card detected #");
    // turn the four byte UID of a mifare classic into a single variable #
    cardidentifier = uid[3];
    cardidentifier <<= 8; cardidentifier |= uid[2];
    cardidentifier <<= 8; cardidentifier |= uid[1];
    cardidentifier <<= 8; cardidentifier |= uid[0];
    Serial.println(cardidentifier);

    if (cardidentifier == 170923268) { //update with your RFID identifier!
      Serial.println("recognized!");
      
    }
  }
}
