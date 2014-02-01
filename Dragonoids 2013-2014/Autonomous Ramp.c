#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     motorMux,       sensorI2CMuxController)
#pragma config(Sensor, S2,     ultrasonic,     sensorSONAR)
#pragma config(Sensor, S3,     gyroSensor,     sensorAnalogInactive)
#pragma config(Sensor, S4,     IRSeeker,       sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     rearRight,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     rearLeft,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     armMotor,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     flagMotor,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    flagRaiserExtender,   tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    blockPusher,          tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    autoArm,              tServoStandard)
#pragma config(Servo,  srvo_S1_C4_6,    autoBlock,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-irseeker-v2.h"
#include "drivers/hitechnic-gyro.h"
#include "JoystickDriver.c"

float heading = 0;

// Motor control functions
void stopMotors() {
	motor[frontLeft] = 0;
  motor[frontRight] = 0;
	motor[rearLeft] = 0;
	motor[rearRight] = 0;
}
void leftSidePower(int power) {
	motor[frontLeft] = power;
	motor[rearLeft] = power;
}
void rightSidePower(int power) {
	motor[frontRight] = power;
	motor[rearRight] = power;
}


// Task for the gyro sensor which sets the global variable
// heading on each update
task gyro() {
	float rotSpeed = 0;
  heading = 0;

  // Reset the timer
  // USES TIMER 4
  // DO NOT USE TIMER 4 ELSEWHERE IN THIS PROGRAM
  time1[T4] = 0;
  while (true)
  {
    // Wait until 20ms has passed
    while (time1[T4] < 20)
      wait1Msec(1);

    // Reset the timer
    time1[T4] = 0;

    // Read the current rotation speed
    rotSpeed = HTGYROreadRot(gyroSensor);

    // Calculate the new heading by adding the amount of degrees
    // we've turned in the last 20ms
    // If our current rate of rotation is 100 degrees/second,
    // then we will have turned 100 * (20/1000) = 2 degrees since
    // the last time we measured.
    heading += rotSpeed * 0.02;

    // Display our current heading on the screen
    eraseDisplay();
    nxtDisplayCenteredBigTextLine(4, "%3.2f", heading);
	}
}
task main() {
	servoChangeRate[flagRaiserExtender] = 5;
	servoChangeRate[autoBlock] = 5;
	servoChangeRate[autoArm] = 3;

	servo[flagRaiserExtender] = 220;
	servo[blockPusher] = 255;
	servo[wrist] = 200;

	servo[autoArm] = 1;
	servo[autoBlock] = 200;

	// Calibrate the gyro, make sure you hold the sensor still
  HTGYROstartCal(gyroSensor);
  nxtDisplayTextLine(4, "Gyro calibration completed");

  nxtDisplayTextLine(6, "Waiting for start...");
  PlaySound(soundUpwardTones);

  wait1Msec(5000);
 	//waitForStart();
  eraseDisplay();
  // Spawn the gyro heading task
  StartTask(gyro, kHighPriority);
	const int power = 30;
	const int turnPower = 20;

	rightSidePower(-power);
	leftSidePower(-power);
	wait1Msec(1200);
	stopMotors();

	// -45 degrees
  while (heading < 14) {
  	rightSidePower(-turnPower);
  	leftSidePower(turnPower);
	}

	rightSidePower(-power);
	leftSidePower(-power);
	wait1Msec(1700);
	stopMotors();

	// Drop a block
	const int autoArmMoveTo = 250;
	const int autoBlockMoveTo = 1;
	servo[autoArm] = autoArmMoveTo;
	while (ServoValue[autoArm] < autoArmMoveTo) {
		wait1Msec(20);
	}
	servo[autoBlock] = 1;
	while (ServoValue[autoBlock] > autoBlockMoveTo) {
		eraseDisplay();
		nxtDisplayCenteredTextLine(5, "BlockMoveTo: %d", ServoValue[autoBlock]);
		wait1Msec(20);
	}
	// Retract arm
	servo[autoBlock] = 200;
	while (ServoValue[autoBlock] < 200) {
		wait1Msec(20);
	}
	servo[autoArm] = 1;
	while (ServoValue[autoArm] > 1) {
		wait1Msec(20);
	}

	PlaySound(soundDownwardTones);
}