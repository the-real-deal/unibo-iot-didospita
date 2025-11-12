#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <vector>

/*BUTTON PIN*/
#define pinButton1 2
#define pinButton2 3
#define pinButton3 4
#define pinButton4 5

/*LED PIN*/
#define pinLedR 7
#define pinLedG1 8
#define pinLedG2 9
#define pinLedG3 10
#define pinLedG4 11

/*POTENZIOMETER PIN*/
#define potPin A0
#define DEBOUNCE_TIME 40

/*int prevts = 0;
int numFiltered = 0;*/

std::vector<int> numbers = {1, 2, 3, 4};
list<int> rdSeq;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0X27,20,4);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  while (!Serial);
  
  /*BUTTON SETUP*/
  /*al posto di inc ci va turn_on_led*/
  attachInterrupt(digitalPinToInterrupt(pinButton1), inc, RISING);
  attachInterrupt(digitalPinToInterrupt(pinButton2), inc, RISING);
  attachInterrupt(digitalPinToInterrupt(pinButton3), inc, RISING);
  attachInterrupt(digitalPinToInterrupt(pinButton4), inc, RISING);

  
  /*LED SETUP*/
  pinMode(pinLedR, OUTPUT);
  pinMode(pinLedG1, OUTPUT);
  pinMode(pinLedG2, OUTPUT);
  pinMode(pinLedG3, OUTPUT);
  pinMode(pinLedG4, OUTPUT);
  
  /*I2C SETUP*/
  i2c_scan();
  lcd.init();
  lcd.backlight();
}

void loop() {
  /*AIUTO SPITA COME CAZZO DEVO GESTIRE TUTTO??*/
  /*Questione del deepsleep*/
  /*Usare uno switch con enum o state variable*/
  
  
  /*Cloni la lista con dentro 1234*/
  /*Shuffle List di quella clonata*/
  rdSeq = numbers.copy().random_shuffle(numbers.begin(),numbers.end());
  /*Display sequence*/
  /*Interrupts*/
  
  
  /*lcd.setCursor(0,0);
  lcd.print("GET IN THE GAME");*/
}

/*void inc() {
  long ts = millis();
  if ((ts - prevts) > DEBOUNCE_TIME) {
    prevts = ts;
  } else {
    numFiltered++;
  }
}*/

void turn_off_leds() {
	
}

void turn_on_specific_led(int pinButton) {
  /*Spegni tutti i led accessi 
  (L'idea è quella di lasciare il led accesso fino
  al prossimo click, se non si vuole così allora si mette
  un delay)*/
  /*Controlli il pinPremuto con il primo numero nella lista.
  Se non è corretto cambi stato della partita a game over
  Se è la lista diventa vuota allora vuol dire che si ha vinto
  e quindi si mette lo stato in vittoria*/
  
  /*Se non va che i define usare i numeri o trovare un altro
  modo*/
  turn_off_leds();
  switch (pinButton) {
  	case pinButton1:
    	digitalWrite(pinLedG1, HIGH);
    	break;
    case pinButton2:
    	digitalWrite(pinLedG2, HIGH);
    	break;
    case pinButton3:
    	digitalWrite(pinLedG3, HIGH);
    	break;
    case pinButton4:
    	digitalWrite(pinLedG4, HIGH);
    	break;
    	
    
  }
}

void i2c_scan() {
  Serial.println("\nI2C Scanner");
  int found = 0;
  while(found == 0) { 
    Serial.println("Scanning...");
    int nDevices = 0;
    for (byte address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16) Serial.print("0");
        Serial.print(address, HEX);
        Serial.println(" !");
        nDevices++;
        lcd = LiquidCrystal_I2C(address,20,4);
        found = 1;
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
  }
}