#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     motorMux,       sensorNone)
#pragma config(Sensor, S2,     colorSensor,    sensorCOLORFULL)
#pragma config(Sensor, S3,     IRSeeker,       sensorI2CCustom)
#pragma config(Sensor, S4,     gyroSensor,     sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motor2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor3,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motor4,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     armMotor,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     unlabeledMotor, tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

#include "drivers/hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"

// START THE ROBOT CENTERED IN THE RED BOX

// Motor control functions
void forwardMotors(int power) {
  motor[motor1] = power;
  motor[motor2] = power;
  motor[motor3] = power;
  motor[motor4] = power;
}
void stopMotors() {
  forwardMotors(0);
}
void rightSidePower(int power) {
	motor[motor2] = power;
	motor[motor4] = power;
}
void leftSidePower(int power) {
	motor[motor1] = power;
	motor[motor3] = power;
}

const int stopDistance = 46; // centimeters (1.5 feet)
task main()
{
	nMotorEncoder[armMotor] = 1000; // Staight up
	// Closed to begin with
	servo[servo1] = 270;
	servo[servo2] = 0;
	waitForStart();

	ClearTimer(T1);
  const int maxRunTime = 20000;
  int runTime = 0;

  while (runTime < maxRunTime) {
  	runTime = time1[T1];
  	int IRDir = HTIRS2readACDir(IRSeeker);
		if (IRDir == -1) {
			return;
		}
		if (IRDir < 5) {
  		forwardMotors(22);
  	}
  	if (IRDir >= 5) {
  		stopMotors();
  		break;
  	}
	}
	// Turn 90 degrees clockwise
	int powerLevel = 128 / 6;
	rightSidePower(powerLevel);
	leftSidePower(-powerLevel);
	wait1Msec(2000);
	stopMotors();
	motor[armMotor] = -15;
	wait1Msec(1000);
	motor[armMotor] = 0;
	// Open claw
	servo[servo1] = 177;
	servo[servo2] = 70;
}
