
// device_bebe_v1
// spawned: 14.1.14
// belongs on an atmega644:
// 4kB SRAM
// 2kB EEPROM
// 64kB FLASH ("Sketch Storage")
#include <stdarg.h>
#include <stdlib.h>
#include <Arduino.h>
/*#include <DS1307.h> //TinyRTC
#include <Wire.h> //TinyRTC dependancy
#include <OneWire.h> //TinyRTC dependancy
*/


//LCD part
#define PIN_SCE   7
#define PIN_RESET 6
#define PIN_DC    5
#define PIN_SDIN  4
#define PIN_SCLK  3

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48

static const byte ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};

void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  LcdWrite(LCD_C, 0xB1 );  // Set LCD Vop (Contrast). 
  LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
  LcdWrite(LCD_C, 0x20 );
  LcdWrite(LCD_C, 0x0C );
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

// gotoXY routine to position cursor 
// x - range: 0 to 84
// y - range: 0 to 5

void gotoXY(int x, int y)
{
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.  

}
void drawLine(void)
{
  unsigned char  j;  
   for(j=0; j<84; j++) // top
  {
          gotoXY (j,0);
    LcdWrite (1,0x01);
  }   
  for(j=0; j<84; j++) //Bottom
  {
          gotoXY (j,5);
    LcdWrite (1,0x80);
  }   

  for(j=0; j<6; j++) // Right
  {
          gotoXY (83,j);
    LcdWrite (1,0xff);
  }   
  for(j=0; j<6; j++) // Left
  {
          gotoXY (0,j);
    LcdWrite (1,0xff);
  }

}


// This makes analogRead() go faster.
// Found at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1208715493/11
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int intState;

void setup(){
	//setup bucket with envelopes
	/*	
	   65536 (2^16) addresses of (140 Bytes/chars + 116Bytes of space)

	   bucket.txt on SD

       linenumber sep=.,flags=(s,d,r)
				0.s010100110010010100101--256bytes of randomness--
				1.sd101010010001010010101--256bytes of randomness--
				2.sd101010010001010010101--256bytes of randomness--
				3.r101010010001010010101--256bytes of randomness--
				4.101010010001010010101--256bytes of randomness--
				5.101010010001010010101--256bytes of randomness--
				.
				.
				.
	   legend:
	   	sep=.: the line always begins with a .
	   	flags=s: the message was sent using this pad, marks this pad as "DO NOT RE-USE"
	   	flags=d: the message received an ACK when it was sent
	   	flags=r: this message id was received and decrypted, marks this pad as "DO NOT RE-USE"

	*/

  //nokia LCD part
  LcdInitialise();
  LcdClear();
  LcdString("device bebe");

	/* open a serial terminal for debugging purposes */
	Serial.begin(57600); 
	delay(1000);

	analogReference(INTERNAL);
	// Set prescale to 16.
	// See http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1208715493/11
	sbi(ADCSRA,ADPS2);
	cbi(ADCSRA,ADPS1);
	cbi(ADCSRA,ADPS0);


	//attach audio receive interrupt port 
	attachInterrupt(0, receiveAudio, RISING);

	//interrupt and change state if "new message" button was pressed
	attachInterrupt(1, newButtonPressed, RISING);

  pinMode(12,OUTPUT);//slope indicator
}

int audioInNew = 0, audioInOld = 0, audioInSlope = 0;
bit[] bReceived;


void loop(){
  LcdClear();
  gotoXY(0,0);


	switch (intState){
	    case 0:
	   	  // listen for audio and buffer if the START sequence is seen

	      // while receive audio
	      	/*	
	      		1010101010101010			  0000000000000000 			    0000000000000000000000000.....
				[START sequence] [100ms wait] [  ID 0-65536  ] [100ms wait] [ bitstream 140x8b = 1120kBit]
				
	      	*/
	      	// decode audio to binary, once end sequence is received, stop listening
	      	// send ACK [receivedID 0-65556] onto the wire
	      	// XOR with the matching envelope ID
        //LcdCharacter((char)edge);
        //LcdClear();
        //LcdCharacter('.');

        //10 b/s == 0.1s/b => peakCount per LOW = 120, per HIGH = 220
        
        //sample for 1120bits
        for(int i; i<1121; i++){
          if(peakCount > 120){
            //high bit
            bReceived[i] = 1;

          }else{
            //low bit
            bReceived[i] = 0;

          }
          peakCount = 0;
          int startSampleBit = millis();
          do{
            //sample the waveform here
            audioInOld = audioInNew;
            audioInNew = analogRead(0);

            //detect leading edge
            if(audioInOld < audioInNew){
              //leading
              audioInSlope = 1;
            }else{ 
              if(audioInSlope == 1){
                peakCount++;
                //set to falling
                audioInSlope = 0;
              }
            }
            int timeSampleBit = millis() - startSampleBit;
          }while(timeSampleBit >= 100);
          //save the bit and go to next

          //make a limit
        }



        if(peakCount > 120 && (oldPeak - newPeak) == 8){ //1200Hz => T = 0.000833333s
          //register a 0
          peakCount = 0;
          //shift bit in receive byte by 1
          bReceived = bReceived << 1;
        }

        if(peakCount > 220 && (oldPeak - newPeak) == 4){ //2200Hz => T = 0.000454545s
          //register a 1
          peakCount = 0;
          //shift bit in receive byte by 1 and add 1
          bReceived = bReceived << 1;
          bReceived = (bReceived + B00000001);

        }

        //write every completed byte into eeprom



	      break;

	    case 1:
	      // send audio

		      // take the message from the queue
		      // send it on to the wire
		      // wait for acknowledge
		      	//decode the ACK to the ID, match it with the sent messages 
	      break;

	    case 2:
	      // get keyboard input and wait for a "send" command
		      // calculate a fingerprint/hash of this message, store it (receiver can use this to verify its authenticity)
		      // when "send" command received, continue to encrypt the message with rsa (use fingerprint of previous message, if none send plain)
		      // 
		      // after encrypting with rsa, take an envelope and XOR the message with it
		      // store message in queue with its id and startsequence attached, ready to be sent

	      break;

	    default:

	      // if "new message" button was pressed, 

	      break;
	}
   
}


void newButtonPressed(){
	intState = 2;
}