#include "SBUS.h"
SBUS x8r(Serial);
uint16_t channels[16];
bool failSafe;
bool lostFrame;

// PINS
int pin_A1 = 2; // AO1
int pin_A2 = 3; // AO2
int pin_ST = 4; // STBY
int pin_AP = 5; // PWM A
int pin_BP = 6; // PWM B
int pin_B1 = 7; // BO1
int pin_B2 = 8; // BO2

int deadband = 3;
int fi,ti,a1,a2,b1,b2,ap,bp;

void setup() {
//  Serial.begin(115200);
//  delay(1000);
  pinMode(pin_AP, OUTPUT);
  pinMode(pin_BP, OUTPUT);
  pinMode(pin_A1, OUTPUT);
  pinMode(pin_A2, OUTPUT);
  pinMode(pin_ST, OUTPUT);
  pinMode(pin_B1, OUTPUT);
  pinMode(pin_B2, OUTPUT);
  x8r.begin();
  digitalWrite(pin_ST, HIGH);
//  Serial.println("Ready!");
}

void loop() {
  if (x8r.read(&channels[0], &failSafe, &lostFrame)) {

    // mapping SBUS raw inputs to -127 ~ 127
    fi = map(channels[1], 179, 1811, -127, 127);
    ti = map(channels[2], 179, 1811, -127, 127);
    if ( (fi > (-1 * deadband)) && (fi < deadband) ) { fi = 0; }
    if ( (ti > (-1 * deadband)) && (ti < deadband) ) { ti = 0; }

    int a = fi;
    int b = fi;
    float t = abs(ti);
    float wp = (127.0 - abs(fi)) / 127.0;
    float wm = (127.0 - abs(fi)) / 127.0;
    if (fi > 0) {
      wm = 1.0;
    }
    else if (fi < 0) {
      wp = 1.0;
    }
    if (ti > 0) {
      a += t * wp;
      b -= t * wm;
    }
    else if (ti < 0) {
      a -= t * wm;
      b += t * wp;
    }
    a1 = (a > 0) ? HIGH : LOW;
    a2 = (a < 0) ? HIGH : LOW;
    b1 = (b > 0) ? HIGH : LOW;
    b2 = (b < 0) ? HIGH : LOW;
    ap = map(abs(a), 0, 127, 0, 255);
    bp = map(abs(b), 0, 127, 0, 255);
//    ap = max(0, min(255, ap));
//    bp = max(0, min(255, bp));
    
//    Serial.print(channels[1]);
//    Serial.print(" ");
//    Serial.print(channels[2]);
//    Serial.print(" ");
//    Serial.print(a1);
//    Serial.print(a2);
//    Serial.print(" ");
//    Serial.print(ap);
//    Serial.print(" ");
//    Serial.print(b1);
//    Serial.print(b2);
//    Serial.print(" ");
//    Serial.print(bp);
//    Serial.println();
    
    digitalWrite(pin_A1, a1);
    digitalWrite(pin_A2, a2);
    analogWrite(pin_AP, ap);
    digitalWrite(pin_B1, b1);
    digitalWrite(pin_B2, b2);
    analogWrite(pin_BP, bp);
  }
}

