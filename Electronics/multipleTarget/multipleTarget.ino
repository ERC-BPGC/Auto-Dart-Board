#include <TeensyStep.h>

// Defining motor pins
const int stepPin1 = 2, dirPin1 = 4;  // Vertical motor 1
const int stepPin2 = 6, dirPin2 = 8;  // Vertical motor 2
const int stepPin3 = 10, dirPin3 = 12; // Horizontal motor

Stepper motor1(stepPin1, dirPin1);
Stepper motor2(stepPin2, dirPin2);
Stepper motor3(stepPin3, dirPin3);
StepControl controller;

// Declaring and initializing required variables
volatile float homeX = 1000, homeY = 1500;  // Home position in steps
volatile float currentX = homeX, currentY = homeY;  // Track current position
volatile float xtar = 0, ytar = 0;
volatile float maxSpeed = 2000, maxAccel = 500;
bool newCommandReceived = false;

void setup() {
    Serial.begin(115200);

    // Initial motor settings
    motor1.setMaxSpeed(maxSpeed);
    motor1.setAcceleration(maxAccel);
    motor2.setMaxSpeed(maxSpeed);
    motor2.setAcceleration(maxAccel);
    motor3.setMaxSpeed(maxSpeed);
    motor3.setAcceleration(maxAccel);
}

// Sets motor to home position and defines it as the new (0,0)
/*void setHome() {
    motor1.setMaxSpeed(5000);
    motor1.setAcceleration(1000);
    motor2.setMaxSpeed(5000);
    motor2.setAcceleration(1000);
    motor3.setMaxSpeed(5000);
    motor3.setAcceleration(1000);

    // Move motors to the home position
    motor1.setTargetAbs(homeY);
    motor2.setTargetAbs(homeY);
    motor3.setTargetAbs(homeX);

    controller.move(motor1, motor2, motor3);

    // Update current position
    currentX = homeX;
    currentY = homeY;

    Serial.println("done");  // Notify Python that the move is complete
}*/

// Moves motor to a given target position smoothly
void moveToTarget(float xtar, float ytar) {
    float xsteps = (xtar * 100);  // Convert coordinates to steps
    float ysteps = (ytar * 100);

    motor1.setMaxSpeed(maxSpeed);
    motor2.setMaxSpeed(maxSpeed);
    motor1.setAcceleration(maxAccel);
    motor2.setAcceleration(maxAccel);
    motor3.setMaxSpeed(maxSpeed);
    motor3.setAcceleration(maxAccel);

    motor1.setTargetAbs(ysteps);
    motor2.setTargetAbs(ysteps);
    motor3.setTargetAbs(xsteps);

    controller.move(motor1, motor2, motor3);

    // Update current position
    currentX = xsteps;
    currentY = ysteps;

    Serial.println("done");  // Notify Python that the move is complete
}

void loop() {
    // Check if a new coordinate has been provided
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        float speed, accel;

        if (sscanf(command.c_str(), "%f %f %f %f", &xtar, &ytar, &speed, &accel) == 4) {
            maxSpeed = speed;
            maxAccel = accel;
            newCommandReceived = true;
        }
    }

    // If new coordinates have been received, update motion
    if (newCommandReceived) {
        newCommandReceived = false;

        moveToTarget(xtar, ytar);
        
    }
}
