#include <vector>
#include <cmath>
#include "Delay.h"

// initialise an instance of the class
void Delay::init(int sampleRate, float bufferSizeSeconds) {
	this->sampleRate = sampleRate;
	this->bufferSizeSeconds = bufferSizeSeconds;
	
	bufferSizeSamples = bufferSizeSeconds * sampleRate;
	buffer.resize(bufferSizeSamples, 0.0f);
	
	writePointer = 0;
	readPointer = 0.0f;
}

// process input sample, output delayed sample
float Delay::process(float input, float delayTimeSeconds, float feedback, float dryGain, float wetGain) {
	
	// limiting for safety
	if(dryGain > 1.0f) dryGain = 1.0f; 
    if(wetGain > 1.0f) wetGain = 1.0f; 
	if(feedback > 0.95f) feedback = 0.95f;
	if(delayTimeSeconds > bufferSizeSeconds) delayTimeSeconds = bufferSizeSeconds;
	
	// smooth delay time to reduce clicking when altering delay time in real-time
	float smoothingAmount = 0.0001f;
	float smoothedDelayTime = delayTimeSeconds; // in seconds
	
	smoothedDelayTime += smoothingAmount * (delayTimeSeconds - smoothedDelayTime); 
	// when smooth time == delay time it adds 0, so won't blow up
	
	// convert delay time in seconds to no. ofsamples
	int delayTimeSamples = smoothedDelayTime * sampleRate; 
	
	// read from delay buffer
    readPointer = writePointer - delayTimeSamples;
    if(readPointer < 0.0f) readPointer += bufferSizeSamples;
    
    int index1 = (int)readPointer;
    int index2 = (readPointer + 1) % bufferSizeSamples;
    float fraction = readPointer - index1;
    // interpolate between buffer values to reduce clicking when altering delay time in real-time
	float delayedOutput = buffer[index1] * (1.0f - fraction) + buffer[index2] * fraction; 
	
	// compress amplitude as it approches 1 to reduce excess effect of feedback
	float attenuatedSample = input + (delayedOutput * feedback);
    attenuatedSample = std::tanh(attenuatedSample); 
    
    // write to delay buffer
    buffer[writePointer] = attenuatedSample;
    writePointer = (writePointer +1) % bufferSizeSamples;
    
    // wrap pointers so they loop back to the start after reaching the end/start of the buffer
    // avoids segmentation error - gDelayBuffer trying to access memory not allocated to it (index > it's size)
    
    return (input * dryGain) + (delayedOutput * wetGain);
}