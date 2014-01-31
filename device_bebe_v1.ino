
// device_bebe_v1
// spawned: 14.1.14
// belongs on an atmega644:
// 4kB SRAM
// 2kB EEPROM
// 64kB FLASH ("Sketch Storage")
#include <stdarg.h>
#include <Arduino.h>
#include <DS1307.h> //TinyRTC
#include <Wire.h> //TinyRTC dependancy
#include <OneWire.h> //TinyRTC dependancy



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


// This makes analogRead() go faster.
// Found at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1208715493/11
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int intState;

// http://www.arduino.cc/playground/Main/Printf
void p(const char *fmt, ... ) {
  char tmp[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, 128, fmt, args);
  va_end(args);
  Serial.print(tmp);
}

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
  LcdString("device bebe ;)");

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
	//attachInterrupt(0, receiveAudio, RISING);

	//interrupt and change state if "new message" button was pressed
	attachInterrupt(1, newButtonPressed, RISING);
}

const int LOW_THRESHOLD = 0;
const int HIGH_THRESHOLD = 360;


inline bool readBit() {
  while (true) {
    int a = analogRead(A0);
    if (a <= LOW_THRESHOLD) {
      return false;
    }
    if (a >= HIGH_THRESHOLD) {
      return true;
    }
  }
}

inline void waitForFallingEdge() {
  while (!readBit()) {
  }
  while (readBit()) {
  }
}

inline void waitForRisingEdge() {
  while (readBit()) {
  }
  while (!readBit()) {
  }
}

float determine_frequency() {
  const int SAMPLE_COUNT = 128;
  unsigned long samples[SAMPLE_COUNT];

  for (int i = 0; i < SAMPLE_COUNT; ++i) {
    unsigned long t = micros();
    waitForRisingEdge();
    waitForFallingEdge();
    samples[i] = micros() - t;
  }
  float total = 0;
  for (int i = 0; i < SAMPLE_COUNT; ++i) {
    total += samples[i];
  }
  float average = total / SAMPLE_COUNT;
  float frequency = 1000000.0 / average;
  return frequency;
}


bool is_weird_packet(uint8_t packet_number) {
  return packet_number == 0 || packet_number == 129;
}

void hang() {
  while (true) {
  }
}




void loop(){
  LcdClear();
  LcdString("device bebe ;)");
	int intState = 0;


			float frequency = determine_frequency();
		  	unsigned long usec = 1000000 / frequency;
		  	unsigned long usec_low = usec * 75 / 100;
		  	unsigned long usec_high = usec * 150 / 100;

		  	Serial.println(frequency);


  enum {
    READ_SYNC = 0,
    READ_PROLOGUE,
    READ_PACKET,
    READ_EPILOGUE,
  };
  uint8_t state = READ_SYNC;

  uint8_t packets_read = 0;

  uint16_t simple_checksum = 0;
  uint8_t packet_checksum = 0;
  uint16_t total_packet_bytes_read = 0;

  uint8_t byte = 0; // The register into which we shift bits.
  int bit_count = 0; // How many bits we've shifted into the byte.

  const int PACKET_SIZE = 256; // Size of the data packet.
  uint8_t bytes[PACKET_SIZE];
  unsigned long byte_count = 0;

  unsigned long t = micros();
  waitForRisingEdge();
  while (true) {
    waitForFallingEdge();

    // Figure out how long it's been since the last falling edge. Knowing this
    // number is the essence of BFSK. If it's above a certain number, that
    // means we read a zero. Otherwise it's a one. (Or vice-versa, depending
    // on the protocol.)
    unsigned long now = micros();
    unsigned long s = now - t;
    t = now;
    bool is_short = s > usec_low && s < usec_high;

    // READ_SYNC solves the problem of not knowing which bit we're at relative
    // to the start of the next byte. We sync up by reading a bunch of ones,
    // then start shifting into the byte register after reading a zero.
    if (state == READ_SYNC) {
      if (!is_short) {
        state = READ_PROLOGUE;
      }
      continue;
    }

    // Shift the bit into the register.
    byte = byte >> 1;
    byte |= is_short ? 0x80 : 0;

    // Have we completed a byte?
    if (++bit_count == 8) {
      bit_count = 0;

      if (state == READ_PROLOGUE) {
        /*if (byte != 0xA9) { //read start sequence
          p("Error: expected READ_PROLOGUE 0xA9, but got %02x.\n", char);
          hang();
        }*/

        // Prologue was good. Flip on the LED and switch to the packet-reading
        // state.
        digitalWrite(13, HIGH);
        state = READ_PACKET;
        packet_checksum = 0;

        continue;
      }

      // The remaining states all want to remember the bytes read, so store
      // the one we just completed in the byte buffer.
      bytes[byte_count++] = byte;
      Serial.println((char)byte);


      if (state == READ_PACKET) {

        // To further confirm that our proof of concept works, we use a
        // very simple (and quick) checksum.
        //
        // For firmware 0201, decoded length 33024 bytes and
        // shasum 4f1cbd041a166565b654c1ebe50562bf380305d4, the output should
        // be 16330, calculated with the Python script included with this file.
        // We exclude so-called "weird" packets from the checksum.
        //
        // We don't count the first/last packet in the checksum or total bytes,
        // because they aren't considered part of the firmware file.
        bool is_weird = is_weird_packet(packets_read);
        if (!is_weird) {
          simple_checksum += byte;
          packet_checksum += byte;
          ++total_packet_bytes_read;
        }

        // Have we completed a packet?
        if (byte_count == PACKET_SIZE) {
          digitalWrite(13, LOW);
          byte_count = 0;
          if (is_weird) {
            // For some reason, the first packet (the one containing
            // "KORG SYSTEM FILE") and the last packet don't have an epilogue.
            // So go straight to waiting for the next packet to start.
            state = READ_SYNC;
          } else {
            state = READ_EPILOGUE;
          }

          if (++packets_read == 129) { //total packets
            p("Successfully read %d packets. Checksum %u. "
              "Firmware bytes %u.\n", packets_read, simple_checksum,
              total_packet_bytes_read);
            hang();
          }
          continue;
        }
      }
   }}



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


void receiveAudio(){
	intState = 0;
}

void newButtonPressed(){
	intState = 2;
}