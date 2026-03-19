#include <Keyboard.h>
#include <SoftwareSerial.h>

// Built for Arduino Leonardo (ATMega32U4) and tested on DFRobot's Beetle Board - SKU: DFR0282
// mySerial is connected to the TX/RX pads on the bottom of the board (Pins 0 and 1 on the Leonardo) and receives RS-232 data to interpret
// This is made primarily to type keyboard shortcuts and will press every key sent to the buffer before releasing. This means that any text you wish to type needs to be sent with a \n between each character.
// The following modifier key assignments are defined:
// CTRL (left ctrl): Decimal 129 - Hex 81
// ALT (left alt): Decimal 141 - Hex 8D
// OS Key (left Windows/OS): Decimal 143 - Hex 8F
// Shift (left shift): Decimal 144 - Hex 90
// Tab: Decimal 157 - Hex 9D

const unsigned int MAX_MESSAGE_LENGTH = 8;

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
 while (mySerial.available() > 0)
 {
   // Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   // Read the next available byte in the serial receive buffer
   char inByte = mySerial.read();

   // Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     // Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }

   // Full message received...
   else
   {
     // Add null character to string
     message[message_pos] = '\0';

     // Print the message (or do other things)
     Serial.print("Message Received: ");
     Serial.println(message);

     for (int n = 0; n < message_pos - 1; n++)
     {
      if (message[n] == (char)129)
      {
        // Unused ASCII space is being used to receive control keys 
        // Decimal 129 - Hex 81 
        Keyboard.press(KEY_LEFT_CTRL);
        Serial.println("CTRL Pressed");
      }
      else if (message[n] == (char)141)
      {
        // Unused ASCII space is being used to receive control keys 
        // Decimal 141 - Hex 8D 
        Keyboard.press(KEY_LEFT_ALT);
        Serial.println("ALT Pressed");
      }
      else if (message[n] == (char)143)
      {
        // Unused ASCII space is being used to receive control keys 
        // Decimal 143 - Hex 8F 
        Keyboard.press(KEY_LEFT_GUI);
        Serial.println("GUI Key Pressed");
      }
      else if (message[n] == (char)144)
      {
        // Unused ASCII space is being used to receive control keys 
        // Decimal 144 - Hex 90 
        Keyboard.press(KEY_LEFT_SHIFT);
        Serial.println("Shift Pressed");
      }
      else if (message[n] == (char)157)
      {
        // Unused ASCII space is being used to receive control keys 
        // Decimal 157 - Hex 9D 
        Keyboard.press(KEY_TAB);
        Serial.println("Tab Pressed");
      }
      else
      {
        // Send an ASCII character within the used character space
        Keyboard.press((char)message[n]);
        Serial.print((char)message[n]);
        Serial.println(" Pressed");
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