// device_bebe_v1
// spawned: 14.1.14
// belongs on an atmega644:
// 4kB SRAM
// 2kB EEPROM
// 64kB FLASH ("Sketch Storage")

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

	//attach audio receive interrupt port 
	attachInterrupt(0, receiveAudio, RISING);

	//interrupt and change state if "new message" button was pressed
	attachInterrupt(1, newButtonPressed, RISING);
}

void loop(){

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