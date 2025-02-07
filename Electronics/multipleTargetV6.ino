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
volatile float x = 0, y = 0;
volatile float xtar = 0, ytar = 0;
float xsteps, ysteps;
bool newCommandReceived = false;

void setup() {
    Serial.begin(115200);

    // Initial motor settings
    motor1.setMaxSpeed(2000);
    motor1.setAcceleration(500);
    motor2.setMaxSpeed(2000);
    motor2.setAcceleration(500);
    motor3.setMaxSpeed(75000);
    motor3.setAcceleration(12500);
}

// Sets motor to home position and defines it as the new (0,0)
void setHome1() 
{
    motor1.setMaxSpeed(15000);
    motor1.setAcceleration(2500);
    motor2.setMaxSpeed(15000);
    motor2.setAcceleration(2500);
    motor3.setMaxSpeed(15000);
    motor3.setAcceleration(2500);

    xsteps = (8.5-x) * 100;
    ysteps = (15-y) * 100;
    x = 8.5;
    y = 15;
    motor1.setTargetRel(ysteps);
    motor2.setTargetRel(ysteps);
    motor3.setTargetRel(xsteps);
    // Move motors to the home position
    /**motor1.setTargetRel(1500);
    motor2.setTargetRel(1500);
    motor3.setTargetRel(850);**/

    controller.move(motor3);
    controller.move(motor1, motor2);
    Serial.println("Done");
}

void setHome2() 
{
    motor1.setMaxSpeed(15000);
    motor1.setAcceleration(2500);
    motor2.setMaxSpeed(15000);
    motor2.setAcceleration(2500);
    motor3.setMaxSpeed(15000);
    motor3.setAcceleration(2500);

    xsteps = (8.5-x) * 100;
    ysteps = (15-y) * 100;
    x = 8.5;
    y = 15;
    motor1.setTargetRel(ysteps);
    motor2.setTargetRel(ysteps);
    motor3.setTargetRel(xsteps);
    // Move motors to the home position
    /**motor1.setTargetRel(1500);
    motor2.setTargetRel(1500);
    motor3.setTargetRel(850);**/

    controller.move(motor1, motor2,motor3);
    Serial.println("Done");
}

// Moves motor to a given target position smoothly
void moveToTarget(float xtar, float ytar, float accel) 
{
    xsteps = (xtar-x) * 100;  
    ysteps = (ytar-y) * 100; 
    x = xtar;
    y = ytar;

    motor1.setMaxSpeed(1500);
    motor2.setMaxSpeed(1500);
    motor1.setAcceleration(2500);
    motor2.setAcceleration(2500);
    motor3.setMaxSpeed(75000);
    motor3.setAcceleration(accel);

    
    motor1.setTargetRel(ysteps);
    motor2.setTargetRel(ysteps);
    motor3.setTargetRel(xsteps);

    //controller.moveAsync(motor1, motor2, motor3); For when giving multiple targets at before motion stops
    controller.moveAsync(motor1, motor2, motor3);
    Serial.println("Done");
}

void loop() {
    // Check if a new coordinate has been provided
    float speed, accel;
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        

        if (sscanf(command.c_str(), "%f %f %f %f", &xtar, &ytar, &speed, &accel) == 4) 
            newCommandReceived = true;
    }

    // If new coordinates have been received, update motion
    if (newCommandReceived) 
    {
        newCommandReceived = false;

        if (xtar == 100 && ytar == 100) 
        {
            setHome2();
        } 
        else if (xtar == 200 && ytar == 200) 
        {
            setHome1();
        } 
        else
        {
            moveToTarget(xtar, ytar, accel);
        }
    }
}
