/*
 * Trace Case (IGEN330 team 304) 
 * Preliminary test to check Serial sommunication between Pi and Arduino
 * -Zayd Khalidi, 31/03/2024
 */

String data;

void setup() {
  Serial.begin(9600);
}
void loop() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(data);
    delay(10);
  }
}
