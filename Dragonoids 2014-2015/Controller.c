#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     motorMux,       sensorNone)
#pragma config(Sensor, S2,     ultrasonic,     sensorSONAR)
#pragma config(Sensor, S3,     gyroSensor,     sensorAnalogInactive)
#pragma config(Sensor, S4,     IRSeeker,       sensorI2CCustom)
#pragma config(Motor,  motorA,          blockMotor1,   tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          blockMotor2,   tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     rearRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rearLeft,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     armMotor,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     flagMotor,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    flagRaiserExtender,   tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    blockBlocker,         tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    autoArm,              tServoStandard)
#pragma config(Servo,  srvo_S1_C4_6,    autoBlock,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "scaleJoy.h"
#include "JoystickDriver.c"

/* HOW TO CONTROL THE ROBOT USING THE FOLLOWING CODE:

Push left joystick to turn left, right joystick to turn right, and both to move forward.
If one is pushed more than the other, than the robot will go forward and turn slightly.

Gamepad 1 is driving
Gamepad 2 is arm control
*/
void stopMotors() {
	motor[frontLeft] = 0;
  motor[frontRight] = 0;
	motor[rearLeft] = 0;
	motor[rearRight] = 0;
}
void rightSidePower(int power) {
	motor[frontRight] = power;
	motor[rearRight] = power;
}
void leftSidePower(int power) {
	motor[frontLeft] = power;
	motor[rearLeft] = power;
}

const int threshold = 10;
float scaler = 0.4;
bool flagRaiserExtended = false;
int lastJoyButton10 = 0;

void driver() {
	// Function for the 1st gamepad that controls driving
	// Joystick values are 8 bit signed ints (-127 to 128) of the data type byte

	// Power levels
	if (joy1Btn(1))
		scaler = 0.4;
	if (joy1Btn(2))
		scaler = 2.0;
	if (joy1Btn(3))
		scaler = 3.0;
	if (joy1Btn(4))
		scaler = 4.0;

	// Turning in place
	if (joy1Btn(5) == 1) {
		// Counter clockwise turn
		int powerLevel = 128 / 6;
		rightSidePower(powerLevel);
		leftSidePower(-powerLevel);
		return;
	}
	if (joy1Btn(6) == 1) {
		// Clockwise turn
		int powerLevel = 128 / 6;
		rightSidePower(-powerLevel);
		leftSidePower(powerLevel);
		return;
	}

	int forwardAmount = scaler * scaleJoy(joystick.joy1_y1);
	int turningAmount = scaler * scaleJoy(joystick.joy1_x2);
	//if (abs(forwardAmount) < threshold)
	//	forwardAmount = 0;
	//if (abs(turningAmount) < threshold)
	//	turningAmount = 0;
	//forwardAmount /= scaler;
	//turningAmount /= scaler;

	leftSidePower(-(forwardAmount - turningAmount));
	rightSidePower(-(forwardAmount + turningAmount));

	//int rightWheelSpeed = forwardAmount;
	//int leftWheelSpeed = forwardAmount;
	//if (turningAmount > 0)
	//	leftWheelSpeed += turningAmount;
	//if (turningAmount < 0)
	//	rightWheelSpeed -= turningAmount;

	//leftSidePower(leftWheelSpeed);
	//rightSidePower(rightWheelSpeed);
}

void arm() {
	int armAmount = joystick.joy2_y1;
	int wristAmount = joystick.joy2_y2;

	//if (abs(armAmount) < threshold)
	//	armAmount = 0;
	if (abs(wristAmount) < threshold)
		wristAmount = 0;

	armAmount = scaleJoy(armAmount);
	motor[armMotor] = armAmount;

	int degreeChange = 0;
	if (wristAmount < 0)
		degreeChange = -5;
	if (wristAmount > 0)
		degreeChange = 5;
	servoChangeRate[blockBlocker] = 50;

	servo[blockBlocker] = ServoValue[blockBlocker] + degreeChange;
	//servo[flagRaiserExtender] = ServoValue[flagRaiserExtender] - degreeChange;

	// Flag raiser
	int flagMotorSpeed = 0;
	if (joy2Btn(5) == 1 && flagRaiserExtended) {
		flagMotorSpeed = -100;
	}
	if (joy2Btn(6) == 1 && flagRaiserExtended) {
		flagMotorSpeed = 100;
	}
	motor[flagMotor] = flagMotorSpeed;


	// Flag raiser extender
	if (joy2Btn(10) == 1 && lastJoyButton10 == 0) {
		if (flagRaiserExtended) {
			// Retract the flag raiser
			servo[flagRaiserExtender] = 220;
		}
		else {
			// Extend the flag raiser
			servo[flagRaiserExtender] = 0;
		}
		flagRaiserExtended = !flagRaiserExtended;
	}
	lastJoyButton10 = joy2Btn(10);

	// Block pusher
	const int speedPusher = 80;
	if (joy2Btn(7) == 1) {
		motor[blockMotor1] = -speedPusher;
		motor[blockMotor2] = -speedPusher;
	}
	else if (joy2Btn(8) == 1) {
		motor[blockMotor1] = speedPusher;
		motor[blockMotor2] = speedPusher;
	}
	else {
		motor[blockMotor1] = 0;
		motor[blockMotor2] = 0;
	}
}
void datalogging() {
	eraseDisplay();
	// Some of this stuff causes the NXT and RobotC to freeze and crash. Idk what.
	//int encoderValue = nMotorEncoder[armMotor];
	//nxtDisplayTextLine(2, "Encoder: %d", encoderValue);
	//nxtDisplayTextLine(2, "Encoder: %s", "N/A");
	//nxtDisplayTextLine(4, "Wrist: %d", ServoValue[wrist]);
	//nxtDisplayTextLine(5, "Flag: %d", ServoValue[flagRaiserExtender]);
	nxtDisplayTextLine(7, "Pusher: %d", ServoValue[blockBlocker]);
}

task main() {
	servoChangeRate[flagRaiserExtender] = 5;
	servoChangeRate[blockBlocker] = 8;

	servo[flagRaiserExtender] = 220;
	servo[wrist] = 90;
	servo[blockBlocker] = 30;

	servo[autoArm] = 145;
	servo[autoBlock] = 200;
	waitForStart();
	//servo[flagRaiserExtender] = 0;

	while (true) {
		bFloatDuringInactiveMotorPWM = false;
		getJoystickSettings(joystick);
		driver();
		arm();
		datalogging();
		// RobotC function for keeping the robot on
		//alive();
	}
}