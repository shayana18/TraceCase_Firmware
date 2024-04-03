/*
 * Simple tests for the Infrared Signal
 * For an Arduino Nano microcontroller
 * - Zayd Khalidi 14/03/2024
 */

const int DAT = 8;
const int OUT = 6;

void setup() {
  pinMode(6, INPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
}

void loop() {
  // const Vcc
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);

  digitalWrite(DAT, HIGH);
  delay(10);
  digitalWrite(DAT, LOW);
  delay(10);

  
}
