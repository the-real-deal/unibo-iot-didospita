#define pinG 3
#define pinY 7
#define pinW 10
#define pinR 2
#define pinB 5
int pins[] = {pinG,pinY,pinW,pinR,pinB};

void setup() {
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinY,OUTPUT);
  pinMode(pinW,OUTPUT);
}

void loop() {
  for(int i = 0; i<4; i++) {
    digitalWrite(pins[i],HIGH);
    delay(1000);
    digitalWrite(pins[i],LOW);
    delay(500);
  }
  for(int i = 4; i>0; i--) {
    digitalWrite(pins[i],HIGH);
    delay(1000);
    digitalWrite(pins[i],LOW);
    delay(500);
  }
}
