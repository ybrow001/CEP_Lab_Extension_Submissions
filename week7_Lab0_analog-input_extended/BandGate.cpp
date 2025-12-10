#include "BandGate.h"

/* use SpectralMask class to create instances */

// initialise BandGate variable - to be added to bandsVector member of SpectralMask class
BandGate initialiseBandGate(float lowLimit, float highLimit) {
	BandGate b;
	b.lowLimit = lowLimit;
	b.highLimit = highLimit;
	b.active = false;
	
	return b;
};