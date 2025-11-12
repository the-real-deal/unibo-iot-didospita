#include "config.h"
#include "debounce.hpp"
#include "display.hpp"
#include "i2c.hpp"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*int prevts = 0;
int numFiltered = 0;*/

const int numb[] = {1, 2, 3, 4};
int seq[sizeof(numb) / sizeof(numb[0])];
LiquidCrystal_I2C *lcd;

void i2c_callback(const uint8_t address) {
  lcd = create_display(address, LCD_DISPLAY_WIDTH, LCD_DISPLAY_HEIGHT);
}

void generate_sequence() {
  bool gen[5] = {0};
  for (int i = 0; i < 4; i++) {
    int val;
    do {
      val = (rand() % 4) + 1;
    } while (gen[val]);
    gen[val] = true;
    seq[i] = val;
  }
}

void turn_off_leds() {
  digitalWrite(PIN_LED_G1, LOW);
  digitalWrite(PIN_LED_G2, LOW);
  digitalWrite(PIN_LED_G3, LOW);
  digitalWrite(PIN_LED_G4, LOW);
}

void turn_on_specific_led(const int pinButton) {
  if (check_default_debounce()) {
    return;
  }
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

  Serial.print("Button pressed: ");
  Serial.println(pinButton);

  turn_off_leds();
  switch (pinButton) {
  case PIN_BUTTON_1:
    Serial.println("1");
    digitalWrite(PIN_LED_G1, HIGH);
    break;
  case PIN_BUTTON_2:
    Serial.println("2");
    digitalWrite(PIN_LED_G2, HIGH);
    break;
  case PIN_BUTTON_3:
    Serial.println("3");
    digitalWrite(PIN_LED_G3, HIGH);
    break;
  case PIN_BUTTON_4:
    Serial.println("4");
    digitalWrite(PIN_LED_G4, HIGH);
    break;
  }
}

void btn1_pressed() { turn_on_specific_led(PIN_BUTTON_1); }
void btn2_pressed() { turn_on_specific_led(PIN_BUTTON_2); }
void btn3_pressed() { turn_on_specific_led(PIN_BUTTON_3); }
void btn4_pressed() { turn_on_specific_led(PIN_BUTTON_4); }

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Wire.begin();
  while (!Serial)
    ;

  /*BUTTON SETUP*/
  /*al posto di inc ci va turn_on_led*/
  enableInterrupt(PIN_BUTTON_1, btn1_pressed, RISING);
  enableInterrupt(PIN_BUTTON_2, btn2_pressed, RISING);
  enableInterrupt(PIN_BUTTON_3, btn3_pressed, RISING);
  enableInterrupt(PIN_BUTTON_4, btn4_pressed, RISING);

  /*LED SETUP*/
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G1, OUTPUT);
  pinMode(PIN_LED_G2, OUTPUT);
  pinMode(PIN_LED_G3, OUTPUT);
  pinMode(PIN_LED_G4, OUTPUT);

  /* I2C SETUP */
  i2c_scan(i2c_callback);
  interrupts();
}

void loop() {
  /*AIUTO SPITA COME CAZZO DEVO GESTIRE TUTTO??*/
  /*Questione del deepsleep*/
  /*Usare uno switch con enum o state variable*/

  /*Cloni la lista con dentro 1234*/
  /*Shuffle List di quella clonata*/
  /*Display sequence*/
  /*Interrupts*/
  generate_sequence();
  lcd->setCursor(0, 0);
  for (size_t i = 0; i < 4; i++) {
    lcd->print(seq[i]);
  }
  delay(1000);
}
