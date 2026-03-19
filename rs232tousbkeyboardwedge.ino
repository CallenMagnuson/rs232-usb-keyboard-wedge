#include <Keyboard.h>
#include <SoftwareSerial.h>

// Built for Arduino Leonardo (ATMega32U4) and tested on DFRobot's Beetle Board - SKU: DFR0282
// Written by Callen Magnuson and released into the public domain.

// mySerial is connected to digital pins 10 and 11 on the Leonardo and receives RS-232 data to interpret
// Keyboard shortcuts are pressed if a modifier key is sent in the first position. Otherwise text is typed normally.

// Data is expected to come in as ASCII data starting with your first key press. If using modifier keys, data will need to be sent in hexidecimal.
// End of packet is expected to be a newline ("\n" or hex "0d0a").
// Define your maximum packet size with MAX_MESSAGE_LENGTH

// The following modifier key assignments are defined within unused portions of the ASCII table.
// CTRL (left ctrl): Decimal 129 - Hex 81
// ALT (left alt): Decimal 141 - Hex 8D
// OS Key (left Windows/OS): Decimal 143 - Hex 8F
// Shift (left shift): Decimal 144 - Hex 90
// Tab: Decimal 157 - Hex 9D

const unsigned int MAX_MESSAGE_LENGTH = 32;

// Define serial pins and inversion
#define rxPin 11
#define txPin 10
#define invert 1

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin, invert);

void setup() {
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // Start serial connections
  mySerial.begin(9600);
  mySerial.setTimeout(100);
  Serial.begin(9600);
}

void loop() {
  // Check to see if anything is available in the serial receive buffer
  while (mySerial.available() > 0) {
    // Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    // Read the next available byte in the serial receive buffer
    char inByte = mySerial.read();

    // Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) ) {
      // Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }

    // Full message received...
    else {
      // Add null character to string
      message[message_pos] = '\0';

      // Print the message (or do other things)
      Serial.print("Message Received: ");
      Serial.println(message);

      for (int n = 0; n < message_pos - 1; n++) {
        if (message[n] == (char)129) {
          // Unused ASCII space is being used to receive control keys 
          // Decimal 129 - Hex 81 
          Keyboard.press(KEY_LEFT_CTRL);
          Serial.println("CTRL Pressed");
        }
        else if (message[n] == (char)141) {
          // Unused ASCII space is being used to receive control keys 
          // Decimal 141 - Hex 8D 
          Keyboard.press(KEY_LEFT_ALT);
          Serial.println("ALT Pressed");
        }
        else if (message[n] == (char)143) {
          // Unused ASCII space is being used to receive control keys 
          // Decimal 143 - Hex 8F 
          Keyboard.press(KEY_LEFT_GUI);
          Serial.println("GUI Key Pressed");
        }
        else if (message[n] == (char)144) {
          // Unused ASCII space is being used to receive control keys 
          // Decimal 144 - Hex 90 
          Keyboard.press(KEY_LEFT_SHIFT);
          Serial.println("Shift Pressed");
        }
        else if (message[n] == (char)157) {
          // Unused ASCII space is being used to receive control keys 
          // Decimal 157 - Hex 9D 
          Keyboard.press(KEY_TAB);
          Serial.println("Tab Pressed");
        }
        else {
          // Check to see if the first position of the serial input is a modifier, if not, then we press and release the key to type normally
          if (message[0] == (char)157 || message[0] == (char)144 || message[0] == (char)143 || message[0] == (char)141 || message[0] == (char)129) {
            Keyboard.press((char)message[n]);
            Keyboard.release((char)message[n]);
            Serial.print((char)message[n]);
            Serial.println(" Typed");
          }
          else {
            // Send an ASCII character within the used character space
            Keyboard.press((char)message[n]);
            Serial.print((char)message[n]);
            Serial.println(" Pressed");
          }
        }
      }

        // Release Keys after adding modifiers
      delay(10);
      Keyboard.releaseAll();
      Serial.println("Keys Released");

      // Send OK response on mySerial to confirm command was executed
      mySerial.println("OK");

      // Reset for the next message
      message_pos = 0;
    }
  }
}