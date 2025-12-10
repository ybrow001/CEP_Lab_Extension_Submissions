#pragma once 
#include <vector>

class Delay {
	public: 
	Delay() = default; 
	// construct instance w/o arguments
	
	void init(int sampleRate, float bufferSizeSeconds);
	// method to initialise instance
	
	float process(float input, float delayTimeSeconds, float feedback, float dryGain, float wetGain); 
	// method applies delay to input signal, returns delayed output
	
	private:
	int sampleRate = 0;
	float bufferSizeSeconds = 0;
	int bufferSizeSamples = 0;
	std::vector<float> buffer;
	int writePointer = 0;
	int readPointer = 0;
};