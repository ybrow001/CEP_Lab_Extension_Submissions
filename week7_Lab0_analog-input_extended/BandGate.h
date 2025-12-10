#pragma once

/* use SpectralMask class to create instances */

struct BandGate {
	float lowLimit;
	float highLimit; 
	bool active; 
};

BandGate initialiseBandGate(float lowLimit, float highLimit);