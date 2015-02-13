/* Adapted from the C++ version in the Arduino PID library */
/* https://github.com/br3ttb/Arduino-PID-Library */
unsigned long lastTime = 0;
float Input, Output, Setpoint;
float errSum, lastErr;
float kp, ki, kd;
int SampleTime = 100; // 10 updates per second

void PIDCompute() {
	unsigned long now = time1[T3];
	int timeChange = (now - lastTime);
	if (timeChange >= SampleTime) {
		/* Compute all the working error variables */
		float error = Setpoint - Input;
		errSum += error;
		float dErr = (error - lastErr);

		/* Compute PID Output */
		Output = kp * error + ki * errSum + kd * dErr;

		/*Remember some variables for next time*/
		lastErr = error;
		lastTime = now;
	}
}

void PIDSetTunings(float Kp, float Ki, float Kd) {
	float SampleTimeInSec = ((float)SampleTime) / 1000;
		kp = Kp;
		ki = Ki * SampleTimeInSec;
		kd = Kd / SampleTimeInSec;
}

void PIDSetSampleTime(int NewSampleTime) {
	if (NewSampleTime > 0) {
		float ratio  = (float)NewSampleTime / (float)SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}
