/*
Simple tests for the ultrasonic and its ensuing controls
- Zayd Khalidi 14/03/2024
*/

#include <math.h>


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

// PID constants
const double Kp = 0.5;
const double Ki = 0;
const double Kd = 0;
const int Set_Point = 100;
const double windupGuard = 255;
const float iMax = 255;
const float iMin = 0;

// general variables
long durationL;
double distanceL;
long durationR;
double distanceR;
double obstacle[2];
double distanceU;
int distancePWM;

// PID variables
float d_Temp = 0;
float i_Temp = 0;
float PWM_Temp = 166;
float PWM_Duty = 166;

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

  obstacle[0] = distanceL;
  obstacle[1] = distanceR;

//  Serial.print("Distance: ");
//  Serial.println(obstacle[1]);
//  Serial.print("PID: ");
//  Serial.println(PID(obstacle[1]));

  if (distanceR > 800)
    distancePWM = 0;   
  else
    distancePWM = (320*(1 - exp(PID(distanceR))/85) - 51.77); // Normalize user distance to a speed
      
  digitalWrite(ENA, distancePWM);
  digitalWrite(ENB, distancePWM);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
//  // Object avoidance
//  if (distanceR < 50 || distanceL < 50) {
//    int PWMdiff = 255/250 * (distanceR - distanceL);   
//    if (PWMdiff > 0)
//      ENA = distancePWM - PWMdiff;
//    else if (PWMdiff < 0)
//      ENB = distancePWM + PWMdiff;
//  }

  // User angle shift
  // TBD
 
}

double PID(double newVal) {

  float errVal;
  float P;
  float I;
  float D;

  if (newVal > 800)
  newVal = PWM_Duty;
  
  // Something to try: for an array of read vals, we can keep each and decay them according to exp(-4*n/(N-1))
  errVal = (Set_Point - newVal);
  P = -Kp*errVal;
  
  i_Temp += errVal;
  if (i_Temp > windupGuard)
    i_Temp = windupGuard;
  else if (i_Temp < -windupGuard)
    i_Temp = -windupGuard;
    

  if (i_Temp > iMax)
    i_Temp = iMax;
  else if (i_Temp < iMin)
    i_Temp = iMin;

  I = Ki*i_Temp;

  D = Kd*(d_Temp - errVal);
  d_Temp = errVal;

  PWM_Duty = PWM_Temp - (P + I + D);
  if (PWM_Duty > 255)
    PWM_Duty = 255;
  if (PWM_Duty < 5)
    PWM_Duty = 5;

  PWM_Temp = PWM_Duty;
  
  return PWM_Duty;

}
