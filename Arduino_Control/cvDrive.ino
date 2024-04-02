/*
 * Trace Case (IGEN330 team 304) 
 * Preliminary code to test motor control with computer vision values from Raspberry Pi
 * -Jasleen Shnider, 1/04/2024
 */

String data;
int x_val;
int dist; 
const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENB = 9;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}
void loop() {

  readSerial();
  
  while (dist > 100) {
    rightMotor(100);
    leftMotor(100);

    delay(10);
    
    readSerial();
  }

  while (dist > 50) {
    rightMotor(85);
    leftMotor(85);

    delay(10);

    readSerial();
  }

  while (dist < 50) {
    rightMotor(-85);
    leftMotor(-85);

    delay(10);
    
    readSerial();
  }
}

void readSerial() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    int comma_index = data.indexOf(",");
    
    dist = data.substring(0,comma_index).toInt();
    x_val = data.substring(comma_index + 1).toInt();
    delay(10);
  }
}

void rightMotor(int motorSpeed) {
   digitalWrite(ENA, motorSpeed);
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
}

void leftMotor(int motorSpeed) {
    digitalWrite(ENB, motorSpeed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
