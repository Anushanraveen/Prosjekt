#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Variabler:
//Farge for led:
const int blaa = 3;
const int rod = 2;

//Lyd sensor:
const int lydSensor = 12;
int volum;

//DFPlayer:
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
boolean spiller;

//Tilfeldighet:
unsigned long tidNaa;
unsigned int interval;
unsigned long forrigeTid;
unsigned int tilfeldig2;

//Ikke implementert. (lydsensor)
//boolean ferdig = false;
//int teller = 0;

//Sett led til gronn:
void settBlaa(){
  analogWrite(blaa, 255);
  analogWrite(rod, 0);
}

//Sett led til rod:
void settRod() {
  analogWrite(blaa, 0);
  analogWrite(rod, 255);
}

void setup() {
  //DFPlayer + seriel monitor initiering:
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  //RGB led:
  pinMode(blaa, OUTPUT);
  pinMode(rod, OUTPUT);
  //Lydsensor:
  pinMode(lydSensor, INPUT);
  
  //Diverse operasjoner ved oppstart:
  randomSeed(analogRead(0));
  settBlaa();
  interval = 5000;
  
  //Dersom DFPLayer ikke initierer korrekt:
  if (!myDFPlayer.begin(mySoftwareSerial, false)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(25);  //Angir volum. Fra 0 til 30.
}

void loop() {
  //Leser av lydsensor:
  volum = digitalRead(lydSensor);
  
  //Ikke implementert if. (lydsensor)
  //if (ferdig == false) {
    
    //Spill av mp3 tilfeldig:
    tidNaa = millis();
    if (tidNaa - forrigeTid >= interval) {
      tilfeldig2 = random(4, 22);
      myDFPlayer.play(tilfeldig2);
      Serial.println(tilfeldig2);
      
      //Nytt intervell angis med tilfeldig varians:
      interval = random(15000, 20000);
      forrigeTid = tidNaa;
      spiller = true;
      settRod();
       
      //if (volum == 1 && teller < 4) {
        //teller += 1;
      //}
      //else if (volum == 0 && teller < 0) {
        //teller -= 1;
      //}
      
      //if (teller == 0) {
        //ferdig = true;
      //}
    }
    
    //Gaar inn om lyd. 
    //Stopper lyd etter tilfeldig (2 til 5) sekunder:
    if (spiller) {
      if (tidNaa - forrigeTid > random(8000, 11000)) {
        myDFPlayer.pause();
        spiller = false;
        settBlaa();
      }
    }
    
    //Dfplayer:     
    if (myDFPlayer.available()) {
      printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
 //}
   
//Om programmet skal være ferdig:
//else {
  //if (spiller == false) {
    //De tre forste lydfilene er applaus, resten annet.
    //myDFPlayer.play(random(1, 4));
  //}
//}
}

//Feedback mp3:
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
