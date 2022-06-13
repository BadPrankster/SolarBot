// Script for SolarBot V1
// June 2022
// Lucas Wirz-Vitiuk & Enes Karapinar

// Libraries
#include <Servo.h>

// Define actuators
Servo angle;
Servo rotation;

// Define actuator pins
int angle_pin = 9;
int rotation_pin = 10;

// Define sensor pins
int ldr_top_left_pin = A0;
int ldr_top_right_pin = A1;
int ldr_bottom_left_pin = A2;
int ldr_bottom_right_pin = A3;

// Define sensor variables
int ldr_top_left_value = 0;
int ldr_top_right_value = 0;
int ldr_bottom_left_value = 0;
int ldr_bottom_right_value = 0;
int threshold = 35;
int average_top = 0;
int average_bottom = 0;
int average_left = 0;
int average_right = 0;
int difference = 0;

// Define actuator variables
int set_angle = 90;
int set_rotation = 90;
int movement_rate = 1;
int angle_max = 170;
int angle_min = 10;

void setup() {

  // Initiate actuators
  angle.attach(angle_pin);
  angle.write(set_angle);
  rotation.attach(rotation_pin);
  rotation.write(set_rotation);

  // Open serial port
  Serial.begin(9600);
}

void loop() {

  // Input sensor data
  ldr_top_left_value = analogRead(ldr_top_left_pin);
  ldr_top_right_value = analogRead(ldr_top_right_pin);
  ldr_bottom_left_value = analogRead(ldr_bottom_left_pin);
  ldr_bottom_right_value = analogRead(ldr_bottom_right_pin);

  // Output sensor data
  Serial.print("LDR Top Left - Value = ");
  Serial.println(ldr_top_left_value);
  Serial.print("LDR Top Right - Value = ");
  Serial.println(ldr_top_right_value);
  Serial.print("LDR Bottom Left - Value = ");
  Serial.println(ldr_bottom_left_value);
  Serial.print("LDR Bottom Right - Value = ");
  Serial.println(ldr_bottom_right_value);
  Serial.println();

  // Calculate average
  average_top = (ldr_top_left_value + ldr_top_right_value) / 2;
  average_bottom = (ldr_bottom_left_value + ldr_bottom_right_value) / 2;
  average_left = (ldr_top_left_value + ldr_bottom_left_value) / 2;
  average_right = (ldr_top_right_value + ldr_bottom_right_value) / 2;

  // Output average
  Serial.print("Average Top = ");
  Serial.println(average_top);
  Serial.print("Average Bottom = ");
  Serial.println(average_bottom);
  Serial.print("Average Left = ");
  Serial.println(average_left);
  Serial.print("Average Right = ");
  Serial.println(average_right);
  Serial.println();

  // Check and set angle
  difference = average_bottom - average_top;
  if (abs(difference) > threshold) {
    if (difference > 0 && set_angle < angle_max) {
      set_angle += movement_rate;
      Serial.println("Angle heightened");
    }
    if (difference < 0 && set_angle > angle_min) {
      set_angle -= movement_rate;
      Serial.println("Angle lowered");
    }
    angle.write(set_angle);
    Serial.print("Angle set to ");
    Serial.println(set_angle);
    Serial.println();
  }

  // Check and set rotation
  difference = average_left - average_right;
  if (abs(difference) > threshold) {
    if (set_angle >= 90) {
      if (difference < 0 && set_rotation < 180) {
        set_rotation += movement_rate;
        Serial.println("Rotate left");
      }
      if (difference > 0 && set_rotation > 0) {
        set_rotation -= movement_rate;
        Serial.println("Rotate right");
      }
    } else {
      if (difference > 0 && set_rotation < 180) {
        set_rotation += movement_rate;
        Serial.println("Rotate right");
      }
      if (difference < 0 && set_rotation > 0) {
        set_rotation -= movement_rate;
        Serial.println("Rotate left");
      }
    }
    rotation.write(set_rotation);
    Serial.print("Rotation set to ");
    Serial.println(set_rotation);
    Serial.println();
  }
  
  // Define loop-time
  delay(1000);
  
  // Print line
  Serial.println("---------------------------------");
  Serial.println();
}
