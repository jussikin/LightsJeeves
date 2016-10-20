#define LIGHTRELAY1 6
#define LIGHTRELAY2 7
#define MOVEMENTINPUT1 13     
#define MOVEMENTINPUT2 12 
#define LIGHTLEVEL A0
#define PIXELPIN 11
#define NUMPIXELS 10

#define SKIPTIMEFORLIGHTSENSOR 300000
#define SKIPTIMEFORSTATUSLIGHTS 10000
#define STATEOFF 0
#define STATEINCOMING 1
#define STATEOUTGOING 2
#define STATEGUARDING 3
#define STATEALERT 4

#define GUARDLIGHTS    0,255,0
#define INCOMINGLIGHTS 255,0,255
#define OUTGOINGLIGHTS 0,0,255
#define ALERTLIGHTS    255,0,0

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

SoftwareSerial mySerial(8, 3); // RX, TX
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN,NEO_KHZ800+ NEO_GRB );

void setup() {
  pinMode(LIGHTRELAY1, OUTPUT);
  pinMode(LIGHTRELAY2, OUTPUT);
  pinMode(MOVEMENTINPUT1, INPUT);
  pinMode(MOVEMENTINPUT2, INPUT);
  digitalWrite(LIGHTRELAY2,HIGH);
  digitalWrite(LIGHTRELAY1,HIGH);
  mySerial.begin(9600);
  Serial1.begin(9600);
  strip.begin();
}

int command=0;
int parameter=0;
unsigned long looptimes = 0;
int lightsets = 0;
int status = STATEGUARDING;
int movementState1 = LOW; 
int movementState2 = LOW;


void loop() { // run over and over
  int movement1 =  digitalRead(MOVEMENTINPUT1);
  int movement2 =  digitalRead(MOVEMENTINPUT2);

  if (mySerial.available()) {
  	 int byte;
  	 byte=mySerial.read();
	 if(command==0){
	 	switch(byte){
	 		case 'o':
	 			command=byte;
	 			break;
	 		case 'f':
	 			command=byte;
	 			break;
 			case 's':
	 			command=byte;
	 			break;
	 	}
	 }else{
	 	switch(command){
	 		case 'o':
	 			if(byte=='1')
	 				digitalWrite(LIGHTRELAY1,LOW);
	 			if(byte=='2')
	 				digitalWrite(LIGHTRELAY2,LOW);
	 			command=0;
	 			parameter=0;
	 			break;
	 		case 'f':
	 			if(byte=='1')
	 				digitalWrite(LIGHTRELAY1,HIGH);
	 			if(byte=='2')
	 				digitalWrite(LIGHTRELAY2,HIGH);
	 			command=0;
	 			parameter=0;
	 			break;
	 		case 's':
 				if(byte=='0')
 					status=STATEOFF;
 				if(byte=='1')
 					status=STATEINCOMING;
 				if(byte=='2')
 					status=STATEOUTGOING;
 				if(byte=='3')
 					status=STATEGUARDING;
 				if(byte=='4')
 					status=STATEALERT;
	 		   	command=0;
	 			parameter=0;
	 			break;
	 		default:
	 			command=0;
	 			parameter=0;
	 	}
	 }
  }
  
  //command parsing ended
  //input state change detection starts
  if (movement1== HIGH) {            // check if the input is HIGH
    if ( movementState1 == LOW) {
      Serial1.print("M1");
      movementState1 = HIGH;
    }
  } else {
    if (movementState1 == HIGH){
      movementState1 = LOW;
    }
  }
  
  
 if (movement2== HIGH) {            // check if the input is HIGH
    if ( movementState2 == LOW) {
      Serial1.print("M2");
      movementState2 = HIGH;
    }
  } else {
    if (movementState2 == HIGH){
      movementState2 = LOW;
    }
  }

  if(looptimes%SKIPTIMEFORLIGHTSENSOR==0){
  	setLights(STATEOFF);
  	int lightlevel = analogRead(LIGHTLEVEL)/4;
  	Serial1.write("L");
  	Serial1.write(lightlevel);
  	Serial1.println();
  	setLights(status);
  }
  
  if(looptimes%SKIPTIMEFORSTATUSLIGHTS==0){
  	setLights(status);
  }
  looptimes++;
}


void setLights(int state){
	switch(state){
		case STATEOFF:
			for(int i=0;i<NUMPIXELS;i++){
					strip.setPixelColor(i, 0, 0, 0);
			}
			break;
		case STATEGUARDING:
			for(int i=0;i<NUMPIXELS;i++){
				if((lightsets+i)%5==0){
					strip.setPixelColor(i, GUARDLIGHTS );
				}
				else
					strip.setPixelColor(i, 0, 0, 0);
			}
			break;
		case STATEINCOMING:
			for(int i=0;i<NUMPIXELS;i++){
				if((lightsets+i)%5==0){
					strip.setPixelColor(i, INCOMINGLIGHTS );
				}
				else
					strip.setPixelColor(i, 0, 0, 0);
			}
			break;
		case STATEOUTGOING:
			for(int i=0;i<NUMPIXELS;i++){
				if((lightsets+i)%5==0){
					strip.setPixelColor(i, OUTGOINGLIGHTS );
				}
				else
					strip.setPixelColor(i, 0, 0, 0);
			}
			break;
		case STATEALERT:
			for(int i=0;i<NUMPIXELS;i++){
				if((lightsets+i)%2==0){
					strip.setPixelColor(i, ALERTLIGHTS );
				}
				else
					strip.setPixelColor(i, 0, 0, 0);
			}
			break;
		
	}
	strip.show();
	lightsets++;
}
