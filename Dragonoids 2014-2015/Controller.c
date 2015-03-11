#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S4, HTServo,  none,     none,     none)
#pragma config(Sensor, S2,     armHeight,      sensorSONAR)
#pragma config(Sensor, S3,     gyroSensor,     sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     rightLift,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     leftLift,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     backRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     backLeft,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     frontLeft,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     sweeper,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     none,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C1_1,    tilt,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C1_2,    collectingDoor,       tServoStandard)
#pragma config(Servo,  srvo_S4_C1_3,    leftGrabber,          tServoStandard)
#pragma config(Servo,  srvo_S4_C1_4,    rightGrabber,         tServoStandard)
#pragma config(Servo,  srvo_S4_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

/* HOW TO CONTROL THE ROBOT USING THE FOLLOWING CODE:

Push left joystick to turn left, right joystick to turn right, and both to move forward.
If one is pushed more than the other, than the robot will go forward and turn slightly.

Gamepad 1 is driving
Gamepad 2 is arm control
*/

// Motor control functions
void stopMotors() {
	motor[frontLeft] = 0;
	motor[frontRight] = 0;
	motor[backLeft] = 0;
	motor[backRight] = 0;
}
void applyRightSidePower(int power) {
	motor[frontRight] = power;
	motor[backRight] = power;
}
void applyLeftSidePower(int power) {
	motor[frontLeft] = power;
	motor[backLeft] = power;
}
// Maps controller positions from +-2^7 to -100 to 100
float powerAdjust(int powerLevel) {
	float multiplier = 0.000061 * pow(powerLevel, 2);
	return powerLevel * multiplier * 0.78125; // 0.78125x is the linear function that maps -128 to 127 to -100 to 100
}

const int threshold = 10;
const bool debug = false;

void driver() {
	// Function for the 1st gamepad that controls driving
	// Joystick values are 8 bit signed ints (-127 to 128) of the data type byte
	int forwardAmount = joystick.joy1_y1;
	int turningAmount = joystick.joy1_x2;

	if (abs(forwardAmount) < threshold)
		forwardAmount = 0;
	if (abs(turningAmount) < threshold)
		turningAmount = 0;

	forwardAmount = powerAdjust(forwardAmount);
	turningAmount = powerAdjust(turningAmount);

	int leftSidePower = forwardAmount + turningAmount;
	int rightSidePower = forwardAmount - turningAmount;

	applyLeftSidePower(leftSidePower);
	applyRightSidePower(rightSidePower);

	// Raise and lower the tube-grabbing claws with the top pair of bumperss
	if (joy1Btn(6) == 1) {
		// Raise the claws
		servo[leftGrabber] = ServoValue[leftGrabber] - 1;
		servo[rightGrabber] = ServoValue[rightGrabber] + 1;
	}
	else if (joy1Btn(5) == 1) {
		// Lower the claws
		servo[leftGrabber] = ServoValue[leftGrabber] + 1;
		servo[rightGrabber] = ServoValue[rightGrabber] - 1;
	}
	if (ServoValue[leftGrabber] > 128) {
		servo[leftGrabber] = 128;
	}
	if (ServoValue[rightGrabber] < 128) {
		servo[rightGrabber] = 128;
	}

	if (debug) {
		eraseDisplay();
		nxtDisplayString(4, "Right: %d", rightSidePower);
		nxtDisplayString(5, "Left: %d", leftSidePower);
	}
}

const int tiltPositionStart = 105;
const int doorPositionStart = 255;
bool collectorSpinning = false;

void arm() {
	const int upPower = 55;
	const int downPower = -30;
	// Raise the lift when button 8 is pressed and lower it when button 7 is pressed
	if (joy2Btn(8) == 1) {
		motor[rightLift] = upPower;
		motor[leftLift] = upPower;
	}
	else if (joy2Btn(7) == 1) {
		motor[rightLift] = downPower;
		motor[leftLift] = downPower;
	}
	else {
		motor[rightLift] = 0;
		motor[leftLift] = 0;
	}

	// Use the D-pad for the tilt and door mechanisms
	if (joystick.joy2_TopHat == 0) {
		// Up
		if (time1[T1] % 5 == 0)
			servo[tilt] = ServoValue[tilt] + 1;
	}
	if (joystick.joy2_TopHat == 4) {
		// Down
		if (time1[T1] % 5 == 0)
			servo[tilt] = ServoValue[tilt] - 1;
	}
	if (joy2Btn(1) == 1) {
		// Right
		servo[collectingDoor] = ServoValue[collectingDoor] - 1;
	}
	if (joy2Btn(3) == 1) {
		// Left
		servo[collectingDoor] = ServoValue[collectingDoor] + 1;
	}
	if (ServoValue[collectingDoor] < 110) {
		servo[collectingDoor] = 110;
	}
	if (ServoValue[collectingDoor] > 255) {
		servo[collectingDoor] = 255;
	}
	if (ServoValue[tilt] > 105) {
		servo[tilt] = 105;
	}

	// Spin the ball collector
	if (joy2Btn(2) == 1) {// && time1[T1] > 100) {
	//	collectorSpinning = !collectorSpinning;
	//	ClearTimer(T1);
	//}
	//if (collectorSpinning) {
		motor[sweeper] = 100;
	}
	else if (joy2Btn(4) == 1) {
		motor[sweeper] = -100;
	}
	else {
		motor[sweeper] = 0;
	}
}

task main() {
	/*
	servoChangeRate[flagRaiserExtender] = 5;
	servoChangeRate[blockBlocker] = 8;
	*/
	// Set initial positions of the various servo motors
	servo[tilt] = tiltPositionStart;
	servo[collectingDoor] = doorPositionStart;
	servo[leftGrabber] = 0;
	servo[rightGrabber] = 255;
	servoChangeRate[leftGrabber] = 100;
	servoChangeRate[rightGrabber] = 100;
	servoChangeRate[tilt] = 5;
	//servo[depositDoor] = 0;
	// Not implemented yet
	//servo[rightGrabber] = 0;
	//servo[leftGrabber] = 0;
	bFloatDuringInactiveMotorPWM = false;
	waitForStart();

	// Reset the lift motor encoders
	nMotorEncoder[rightLift] = 0;
	nMotorEncoder[leftLift] = 0;

	while (true) {
		getJoystickSettings(joystick);
		driver();
		arm();
		// RobotC function for keeping the robot on
		//alive();
	}
}
