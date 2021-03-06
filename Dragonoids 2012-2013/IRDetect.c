#pragma config(Sensor, S2,     HTIRS2,         sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-irseeker-v2.h"

task main() {
	int IRDir;
	while (true) {
		IRDir = HTIRS2readACDir(HTIRS2);

		int leftBound = 4;
		int rightBound = 6;
		if (IRDir == 5 || IRDir == leftBound || IRDir == rightBound) {
			// Center peg
			eraseDisplay();
			nxtDisplayCenteredTextLine(4, "Center %d", IRDir);
		}
		else if (IRDir < leftBound) {
			// Left peg
			eraseDisplay();
			nxtDisplayCenteredTextLine(4, "Left %d", IRDir);
		}
		else if (IRDir > rightBound) {
			// Right peg
			eraseDisplay();
			nxtDisplayCenteredTextLine(4, "Right %d", IRDir);
		}
	}
}
