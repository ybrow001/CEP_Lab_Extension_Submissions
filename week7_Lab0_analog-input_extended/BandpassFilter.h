#ifndef BANDPASSFILTER_H
#define BANDPASSFILTER_H

/* adapted from week 4 low pass filter */

//second order pandpass filter

class BandpassFilter {
	public:
		BandpassFilter(float centreFreq, float bandwidth, float sampleRate);
		float process(float input);
		void reset();
	private:
		float b0,b1,b2,a1,a2; //filter coefficients
		float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f; // filter inputs and outputs
};

#endif