#include <Servo.h>

const int trigPin = 9; // const (stathero) onomazo to pin9-> trigpin
const int echoPin = 10; // int statheros stroggilos arithmos

Servo servo1;
Servo servo2;

// NEW: PID settings
float setpoint = 13;    // cm, where you want the ball (middle of beam)
float Kp = 1.0; // float diladi 13.05 exi .
float Ki = 0.01;
float Kd = 0.02;

// NEW: PID memory : ksekinima me 0 error
float integral = 0; //oloklirom
float lastError = 0; // telefteo error
float dt = 0.05;        // 50 ms loop time betwin the loops

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo1.attach(6); // attach is a function that belongs to a servo. That’s why it has a dot .
  servo2.attach(5);
  setTilt(0);           // NEW: start flat
  delay(1000);          // NEW
}

void loop() {
  float distance = readDistance(); //float distance;  a decimal(dekadikos srithmos) number called distance

  // ignore bad readings
  if (distance < 1 || distance > 30) {
    delay(50);
    return;
  }

  // PID calculation
  float error = distance - setpoint;
  integral = integral + error * dt;
  float derivative = (error - lastError) / dt;
  lastError = error;

  float output = Kp * error + Ki * integral + Kd * derivative; //ki olokliroma // kd paragogos
  output = constrain(output, -30, 15);

  setTilt(output);

  // CHANGED: print moved here, after output is calculated
  Serial.print("Distance:");
  Serial.print(distance);
  Serial.print(" Servo1:");
  Serial.println(105 + output);

  delay(50);
}

// NEW: sensor code from Step 1, now as a function
float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // CHANGED: added timeout
  return duration * 0.0343 / 2;
}

// CHANGED: tilt is now a float
void setTilt(float tilt) {
  servo1.write(105 + tilt);
  servo2.write(105 - tilt);   // keep whichever sign worked for you before
}