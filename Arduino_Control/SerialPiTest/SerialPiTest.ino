/*
 * Trace Case (IGEN330 team 304) 
 * Preliminary test to check Serial sommunication between Pi and Arduino
 * -Zayd Khalidi, 31/03/2024
 */

String data;
String x_val;
String dist; 
String string_to_send;

void setup() {
  Serial.begin(9600);
}
void loop() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    int comma_index = data.indexOf(",");
    
    dist = data.substring(0,comma_index);
    x_val = data.substring(comma_index + 1);
    string_to_send = "Dist:" + dist + "x-val:" + x_val;
    Serial.println(string_to_send);
    delay(10);
  }
}
