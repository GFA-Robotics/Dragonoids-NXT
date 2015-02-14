#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S4, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRSeeker,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     gyroSensor,     sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     rightLift,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftLift,      tmotorTetrix, openLoop, reversed, encoder)
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

const int power = 40;

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
void goForward(int forwardTime) {
	applyRightSidePower(power);
	applyLeftSidePower(power);
	wait1Msec(forwardTime);
	stopMotors();
}
void goBackward(int backwardTime) {
	applyRightSidePower(-power);
	applyLeftSidePower(-power);
	wait1Msec(backwardTime);
	stopMotors();
}

task main() {
	PlaySound(soundUpwardTones);
	// Initialize servos
	servo[tilt] = 110;
	servo[collectingDoor] = 255;
	// Retracted
	servo[leftGrabber] = 255;
	servo[rightGrabber] = 0;
 	waitForStart();
  eraseDisplay();

  const int forwardTime = 2500;
  goForward(forwardTime);

}