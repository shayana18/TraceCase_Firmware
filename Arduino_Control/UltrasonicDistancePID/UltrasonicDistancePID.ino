/*
Simple tests for the ultrasonic and its ensuing controls
- Zayd Khalidi 14/03/2024
*/

#include <math.h>

String data;
int x_val;
int x_middle = 320;
int PiDist; 
String string_to_send;

// defines pins numbers
const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENB = 9;

const int trigPinL = 10;
const int echoPinL = 11;

const int trigPinR = 12;
const int echoPinR = 13;

const double K = 0.50;

// PID constants
const double Kp = 0.50;
const double Ki = 0.0;
const double Kd = 0.0;
const double Set_Point = 100.0;
const double iMax = 200;
const double iMin = 0;

// general variables
double durationL;
double distanceL;
double durationR;
double distanceR;
double distancePWM;
bool direct;
double last_error;
double i_term = 0;
double totalDist = 0;


void setup() {
  pinMode(ENA, OUTPUT); // Right Motor PWNM
  pinMode(IN1, OUTPUT); // Right Motor Forwards
  pinMode(IN2, OUTPUT); // Right Motor Backwards
  pinMode(IN3, OUTPUT); // Left Motor Forwards
  pinMode(IN4, OUTPUT); // Left Motor Backwards
  pinMode(ENB, OUTPUT); // Left Motor PWM

  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT); // Sets the echoPin as an Input
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600); // Starts the serial communication
}
void loop() {

  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    int comma_index = data.indexOf(",");
    PiDist = data.substring(0,comma_index).toInt();
    x_val = data.substring(comma_index + 1).toInt();
    Serial.println(PiDist);
    delay(10);
  }
  
  // Clears the trigPin
  digitalWrite(trigPinL, LOW); // 3ums
  delayMicroseconds(2); // 2ums
  digitalWrite(trigPinL, HIGH); // 3ums
  delayMicroseconds(10); // 10ums
  digitalWrite(trigPinL, LOW); //3ums
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationL = pulseIn(echoPinL, HIGH);
  // Calculating the distance
  distanceL = durationL * 0.034 / 2; // ~3ums

  // Clears the trigPin
  digitalWrite(trigPinR, LOW); // 3ums
  delayMicroseconds(2);  // 2ums
  digitalWrite(trigPinR, HIGH); // 3ums
  delayMicroseconds(10); // 10ums
  digitalWrite(trigPinR, LOW); // 3ums
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationR = pulseIn(echoPinR, HIGH);
  // Calculating the distance
  distanceR = durationR * 0.034 / 2; // ~3ums

// Total delay from both R&L = ~50ums

//  // Prints the distance on the Serial Monitor
//  Serial.print("Distance Left: ");
//  Serial.print(distanceL);
//
//  // Prints the distance on the Serial Monitor
//  Serial.print("\t\t Distance Right: ");
//  Serial.println(distanceR);

//  distancePWM = PID(530*(1 - exp(-(distanceR))/85) - 240); // Normalize user distance to a speed

    if(PiDist == 0) {
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);      
    }
    else {
      // User angle shift
      if(x_val < x_middle + 50) {
        analogWrite(ENA, ((PID(PiDist) + K*x_val) > 200) ? 200 : (PID(PiDist) + K*x_val));
        analogWrite(ENB, ((PID(PiDist) - K*x_val) < 0) ? 0 : (PID(PiDist) - K*x_val));
      }
      else if(x_val >  x_middle - 50) {
        analogWrite(ENA, ((PID(PiDist) - K*x_val) < 0) ? 0 : (PID(PiDist) - K*x_val));
        analogWrite(ENB, ((PID(PiDist) + K*x_val) > 200) ? 200 : (PID(PiDist) + K*x_val));
      }
      else {
        analogWrite(ENA, PID(PiDist));
        analogWrite(ENB, PID(PiDist));
      }
    }
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  
//  else {
//    analogWrite(ENA, 0);
//    analogWrite(ENB, 0);
//  }
//  // Object avoidance
//  if (distanceR < 50 || distanceL < 50) { 
//    int PWMdiff = 255/250 * (distanceR - distanceL);   
//    if (PWMdiff > 0)
//      ENA = distancePWM - PWMdiff;
//    else if (PWMdiff < 0)
//      ENB = distancePWM + PWMdiff;
//  }

 
}

double PID (double newVal) {

  double error = Set_Point - newVal;

  i_term += error;
  if (i_term > iMax)
    i_term = iMax;
  else if (i_term < iMin)
    i_term = iMin;
  
  int PWM = 95.0 - Kp*(error) - Kd*(error-last_error) - Ki*(i_term);

  if (PWM > 200)
    PWM = 200;

  last_error = error;
  
  return PWM;
}
