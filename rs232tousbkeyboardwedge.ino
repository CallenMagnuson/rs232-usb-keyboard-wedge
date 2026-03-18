#include <Keyboard.h>
#include <KeyboardLayout.h>

// Built for Arduino Leonardo (ATMega32U4) and tested on DFRobot's Beetle Board - SKU: DFR0282
// Serial1 is connected to the TX/RX pads on the bottom of the board (Pins 0 and 1 on the Leonardo) and receives RS-232 data to interpret
// This is made primarily to type keyboard shortcuts and will press every key sent to the buffer before releasing. This means that any text you wish to type needs to be sent with a \n between each character.
// The following modifier key assignments are defined:
// CTRL (left ctrl): Decimal 129 - Hex 81
// ALT (left alt): Decimal 141 - Hex 8D
// OS Key (left Windows/OS): Decimal 143 - Hex 8F
// Shift (left shift): Decimal 144 - Hex 90
// Tab: Decimal 157 - Hex 9D

const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 digitalWrite(LED_BUILTIN,HIGH);
 delay(10);
 digitalWrite(LED_BUILTIN,LOW);
 delay(20);

 //Check to see if anything is available in the serial receive buffer
 while (Serial1.available() > 0)
 {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial1.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     //Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }

   //Full message received...
   else
   {
     //Add null character to string
     message[message_pos] = '\0';
     //Print the message (or do other things)
     Serial.print("Message Received: ");
     Serial.println(message);

     int message_size = message_pos;

     for (int n = 0; n <= message_size; n++)
     {
      if (message[n] == 129)
      {
        //Unused ASCII space is being used to receive control keys 
        // Decimal 129 - Hex 81 
        Keyboard.press(KEY_LEFT_CTRL);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.println("CTRL Pressed");
      }
      else if (message[n] == 141)
      {
        //Unused ASCII space is being used to receive control keys 
        // Decimal 141 - Hex 8D 
        Keyboard.press(KEY_LEFT_ALT);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.println("ALT Pressed");
      }
      else if (message[n] == 143)
      {
        //Unused ASCII space is being used to receive control keys 
        // Decimal 143 - Hex 8F 
        Keyboard.press(KEY_LEFT_GUI);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.println("GUI Key Pressed");
      }
      else if (message[n] == 144)
      {
        //Unused ASCII space is being used to receive control keys 
        // Decimal 144 - Hex 90 
        Keyboard.press(KEY_LEFT_SHIFT);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.println("Shift Pressed");
      }
      else if (message[n] == 157)
      {
        //Unused ASCII space is being used to receive control keys 
        // Decimal 157 - Hex 9D 
        Keyboard.press(KEY_TAB);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.println("Tab Pressed");
      }
      else
      {
        //Send an ASCII character within the used character space
        Keyboard.press((char)n);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        Serial.print((char)n);
        Serial.println(" Pressed");
      }
     }

     //Reset for the next message
     digitalWrite(LED_BUILTIN,LOW);
     delay(25);
     Keyboard.releaseAll();
     message_pos = 0;
   }
 }
}