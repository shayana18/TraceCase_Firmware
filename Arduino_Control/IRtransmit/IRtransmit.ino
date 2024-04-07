/*
 * Simple tests for the Infrared Signal
 * For an Arduino Nano microcontroller
 * - Zayd Khalidi 14/03/2024
 */

const int DAT = 3;
const int OUT = A1;

void setup() {
 
  pinMode(2, OUTPUT); //D2
  pinMode(DAT, OUTPUT); //D3
  pinMode(OUT, INPUT); //D4
  pinMode(5, OUTPUT); //D5
  pinMode(A0, INPUT);

  Serial.begin(115200);
}

void loop() {
  // const Vcc
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  digitalWrite(DAT, HIGH);
//  delay(10);
//  digitalWrite(DAT, LOW);
//  delay(10);

//  if (digitalRead(OUT) == 1) {
//    Serial.println("IR signal detected");
//  }

  Serial.print("Receiver: ");
  Serial.println(digitalRead(OUT));
  Serial.print("QRD: ");
  Serial.println(digitalRead(A0));

  delay(100);

  
}
