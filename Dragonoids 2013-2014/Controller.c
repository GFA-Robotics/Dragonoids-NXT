#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     motorMux,       sensorI2CMuxController)
#pragma config(Sensor, S2,     IRSeeker,       sensorI2CCustom)
#pragma config(Sensor, S3,     colorSensor,    sensorCOLORFULL)
#pragma config(Sensor, S4,     gyroSensor,     sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motor2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor3,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motor4,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     armMotor,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     bucketMotor,   tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

/* HOW TO CONTROL THE ROBOT USING THE FOLLOWING CODE:

Push left joystick to turn left, right joystick to turn right, and both to move forward.
If one is pushed more than the other, than the robot will go forward and turn slightly.

Gamepad 1 is driving
Gamepad 2 is arm control
*/
void stopMotors() {
	motor[motor1] = 0;
  motor[motor2] = 0;
	motor[motor3] = 0;
	motor[motor4] = 0;
}
void rightSidePower(int power) {
	motor[motor2] = power;
	motor[motor4] = power;
}
void leftSidePower(int power) {
	motor[motor1] = power;
	motor[motor3] = power;
}

const int threshold = 10;
int scaler = 6;

void driver() {
	// Function for the 1st gamepad that controls driving
	// Joystick values are 8 bit signed ints (-127 to 128) of the data type byte

	// Variable speeds
	if (joy1Btn(1) == 1) {
		scaler = 6;
		PlaySound(soundBeepBeep);
	}
	if (joy1Btn(2) == 1) {
		scaler = 4;
		PlaySound(soundBeepBeep);
	}
	if (joy1Btn(3) == 1) {
		scaler = 2;
		PlaySound(soundBeepBeep);
	}
	if (joy1Btn(4) == 1) {
		scaler = 1;
		PlaySound(soundBeepBeep);
	}

	// Turning in place
	if (joy1Btn(5) == 1) {
		// Counter clockwise turn
		int powerLevel = 128 / scaler;
		rightSidePower(-powerLevel);
		leftSidePower(powerLevel);
		return;
	}
	if (joy1Btn(6) == 1) {
		// Clockwise turn
		int powerLevel = 128 / scaler;
		rightSidePower(powerLevel);
		leftSidePower(-powerLevel);
		return;
	}

	int forwardAmount = joy1_y1;
	int turningAmount = joy1_x2;
	if (abs(forwardAmount) < threshold)
		forwardAmount = 0;
	if (abs(turningAmount) < threshold)
		turningAmount = 0;
	forwardAmount /= scaler;
	turningAmount /= scaler;

	int rightWheelSpeed = forwardAmount;
	int leftWheelSpeed = forwardAmount;
	if (turningAmount > 0)
		rightWheelSpeed -= turningAmount;
	if (turningAmount < 0)
		leftWheelSpeed -= turningAmount;

	motor[motor1] = leftWheelSpeed;
	motor[motor3] = leftWheelSpeed;
	motor[motor2] = rightWheelSpeed;
	motor[motor4] = rightWheelSpeed;
}
void arm() {
	// Function for the 2nd gamepad that controls the arm
}

task main() {
	waitForStart();
	//servoChangeRate[armExt] = 1;
	while (true) {
		getJoystickSettings(joystick);
		driver();
		arm();
		alive();
	}
}
