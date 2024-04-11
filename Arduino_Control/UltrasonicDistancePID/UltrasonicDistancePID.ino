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
int disconnected = 0;

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

int spike = 0;
const int maxPWM = 255;

const double Kangle = 0.3;
const double Kobject =0.0;

// PID constants

const double Kmaster = 1.0;
const double Kp = 2.50;
const double Ki = 0.0;
const double Kd = 0.0;
const double Set_Point = 50.0;
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
double delta_t;
double old_t;
const int WSF_Nterms = 1;
double WSF[WSF_Nterms];
double WSF_totalpercent = 0;
double error_array[WSF_Nterms];
int num_same_vals=0;
double old_dist=0;
double time_sameval=0;
double oldtime_sameval=0;

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
  
//  for(int i=0;i<WSF_Nterms;i++){
//    WSF_totalpercent=100*exp(-4*i/(WSF_Nterms-1))+WSF_totalpercent;
//  }
//  for(int i=0;i<WSF_Nterms;i++){
//    WSF[i]=100*exp(-4*i/(WSF_Nterms-1))/WSF_totalpercent;
//  }
//  for(int i=0;i<WSF_Nterms;i++){
//    error_array[i]=0;
//  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
    int comma_index = data.indexOf(",");
    PiDist = data.substring(0,comma_index).toInt();
    x_val = data.substring(comma_index + 1).toInt();
    
      if(PiDist!=old_dist){  // if new PiDist matches old Pi dist, increment nm_same_vals by 1
        oldtime_sameval=millis();
      }
      else{
          time_sameval=millis()-oldtime_sameval;
      }

      if(time_sameval>1000){
         while(PiDist==old_dist){
     analogWrite(ENA, 0);
        analogWrite(ENB, 0);
        
        if (Serial.available() > 0) {
        data = Serial.readStringUntil('\n');
        int comma_index = data.indexOf(",");
        PiDist = data.substring(0,comma_index).toInt();
        }  
      }
     // timeout();
      }
      
//      num_same_vals++;
//    }
//    
//    else{
//      num_same_vals=0;
//    }
//    
//    if(num_same_vals=10){
//    Sameval();
//    } 
    old_dist=PiDist;
    delay(10);
  }
  
//  // Clears the trigPin
//  digitalWrite(trigPinL, LOW); // 3ums
//  delayMicroseconds(2); // 2ums
//  digitalWrite(trigPinL, HIGH); // 3ums
//  delayMicroseconds(10); // 10ums
//  digitalWrite(trigPinL, LOW); //3ums
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  durationL = pulseIn(echoPinL, HIGH);
//  // Calculating the distance
//  distanceL = durationL * 0.034 / 2; // ~3ums
//
//  // Clears the trigPin
//  digitalWrite(trigPinR, LOW); // 3ums
//  delayMicroseconds(2);  // 2ums
//  digitalWrite(trigPinR, HIGH); // 3ums
//  delayMicroseconds(10); // 10ums
//  digitalWrite(trigPinR, LOW); // 3ums
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  durationR = pulseIn(echoPinR, HIGH);
//  // Calculating the distance
//  distanceR = durationR * 0.034 / 2; // ~3ums
//
//// Total delay from both R&L = ~50ums
//
//  // Prints the distance on the Serial Monitor
//  Serial.print("Distance Left: ");
//  Serial.print(distanceL);
//
//  // Prints the distance on the Serial Monitor
//  Serial.print("\t\t Distance Right: ");
//  Serial.println(distanceR);
//
//  distancePWM = PID(530*(1 - exp(-(distanceR))/85) - 240); // Normalize user distance to a speed
//
//    if (spike - millis() > 1000) {
//      delay(250);
//      analogWrite(ENA, maxPWM);
//      analogWrite(ENB, maxPWM);
//      delay(10);
//      spike = millis();
//    }
//    
//
//    if(distanceR < 50){  
//      analogWrite(ENA, maxPWM - Kobject*(50-distanceR));
//    }
//    else if (distanceL < 50) {
//      analogWrite(ENB, maxPWM - Kobject*(50-distanceL));
//    }
//    else {
//      analogWrite(ENA, maxPWM);
//      analogWrite(ENB, maxPWM);
//    }
//    if(disconnected <= 1500) {
//      analogWrite(ENA, 0);
//      analogWrite(ENB, 0);      
//    }
//    else {
      // User angle shift
      if(x_val < x_middle + 75) { 
        analogWrite(ENA, ((PID(PiDist) + Kangle*abs(x_middle-x_val)) > maxPWM) ? maxPWM : (PID(PiDist) + Kangle*abs(x_middle-x_val)));
//        analogWrite(ENA, maxPWM);
        analogWrite(ENB, ((PID(PiDist) - Kangle*abs(x_middle-x_val)) < 0) ? 0 : (PID(PiDist) - Kangle*abs(x_middle-x_val)));
      }
      else if(x_val >  x_middle - 75) {
        analogWrite(ENA, ((PID(PiDist) - Kangle*abs(x_middle-x_val)) < 0) ? 0 : (PID(PiDist) - Kangle*abs(x_middle-x_val)));
//        analogWrite(ENB, maxPWM);
      analogWrite(ENB, ((PID(PiDist) + Kangle*abs(x_middle-x_val)) > maxPWM) ? maxPWM : (PID(PiDist) + Kangle*abs(x_middle-x_val)));
      }
      else {
        analogWrite(ENA, PID(PiDist));
        analogWrite(ENB, PID(PiDist));
      }
}


double PID (int newVal) {
  delta_t = millis() - old_t;
  old_t = millis();

  double error = Set_Point - newVal;
  double diff;
  
//  for(int i=1;i<WSF_Nterms;i++){
//    error_array[WSF_Nterms-(i)] = error_array[WSF_Nterms-(i+1)];
//  }
//  error_array[0] = diff;
//  for(int i=0;i<WSF_Nterms;i++){
//    error += error_array[i]*WSF[i];
//  }
  
  i_term += error;
  
  if (i_term > iMax)
    i_term = iMax;
  else if (i_term < iMin)
    i_term = iMin;
  
  int PWM = 150.0 - Kmaster*(Kp*(error) + Kd*(error-last_error)/delta_t + Ki*(i_term));

  if (PWM > maxPWM)
    PWM = maxPWM;

  last_error = error;

  return PWM;
}

void Sameval(){
      while(PiDist==old_dist){ 
      //While the value read is the same as it was, set motors = 0, keep reading until PiDist does not = old dist
        analogWrite(ENA, 0);
        analogWrite(ENB, 0);
        
        if (Serial.available() > 0) {
        data = Serial.readStringUntil('\n');
        int comma_index = data.indexOf(",");
        PiDist = data.substring(0,comma_index).toInt();
        }  
      }
      num_same_vals=0;
      return;
     }
void timeout(){
  while(PiDist==old_dist){
     analogWrite(ENA, 0);
        analogWrite(ENB, 0);
        
        if (Serial.available() > 0) {
        data = Serial.readStringUntil('\n');
        int comma_index = data.indexOf(",");
        PiDist = data.substring(0,comma_index).toInt();
        }  
      }
  return;
}
