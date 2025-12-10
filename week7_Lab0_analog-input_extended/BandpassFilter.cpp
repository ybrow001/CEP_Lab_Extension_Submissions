#include "BandpassFilter.h"
#include <cmath>

/* adapted from week 4 low pass filter */

//Constructor to initialize the low-pass filter with two stages
BandpassFilter::BandpassFilter(float centreFreq, float bandwidth, float sampleRate) {
    // calculate coefficients
    float Q = centreFreq / bandwidth;
    float omega = 2.0f * M_PI * centreFreq/sampleRate;
    float alpha = sin(omega) / 2.0f * Q;
    
    float b0 = alpha;
    float b1 = 0.0f;
    float b2 = -alpha;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 = 1.0f - alpha;
    
    // normalise coefficients
    this->b0 = b0/a0;
    this->b1 = b1/a0;
    this->b2 = b2/a0;
    this->a1 = a1/a0;
    this->a2 = a2/a0;
    
    // reset inputs and outputs
    x1 = x2 = y1 = y2 = 0.0f;
}

// process input sample, output processed sample
float BandpassFilter::process(float x) {
	// digital biquad difference equation using calculated coefficients and input
	// b_ are input, a_ are output
	
	float y = b0*x + b1*x1 + b2*x2 - a1*y1 - a2*y2;
	
	// update i/os with previous values
	x1 = x;
	x2 = x1;
	y1 = y;
	y2 = y1;
	
    return y;
};

void BandpassFilter::reset() {
	x1 = 0.0f; 
	x2 = 0.0f;
	y1 = 0.0f;
	y2 = 0.0f;	
};
