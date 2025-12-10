#include "NoiseGenerator.h"
#include <cmath>
#include <cstdlib>
#include <vector>

NoiseGenerator::NoiseGenerator( float freqRangeLowIn, float freqRangeHighIn, int sampleRateIn, int numPartials)
: sampleRate(sampleRateIn), freqRangeLow(freqRangeLowIn), freqRangeHigh(freqRangeHighIn) 
{
	Partial p;
	p.freq = freqRangeLow + (rand() / (float)RAND_MAX) * (freqRangeHigh - freqRangeLow);
	p.phase = 0.0f;
	p.amp = 0.001f + (rand() / (float)RAND_MAX) * 0.002f;
	
	partialVector.push_back(p);
};

void NoiseGenerator::updatePartials(float randomProbability, float normalisedLDRValue) {
	// keep randomProbability within range 0 to 1;
	if(randomProbability >= 1.0f)
		randomProbability = 0.99f;
	
	if(randomProbability <= 0.0f)
		randomProbability = 0.01f;
	
	// scale randomProbability by LDR value
	float probability = randomProbability * normalisedLDRValue;
	
	for(auto &p : partialVector) {
		if(rand()/(float)RAND_MAX < probability) {
			p.freq = 20.0f + (rand() / (float)RAND_MAX) * 19980.0f;
			p.amp = 0.001f + (rand() / (float)RAND_MAX) * 0.002f;
		};
        
        p.phase += (2.0f * M_PI * p.freq) / sampleRate;
        if(p.phase > 2.0f * M_PI)
        	p.phase -= 2.0f * M_PI;
    };
};

const std::vector<NoiseGenerator::Partial>& NoiseGenerator::getPartials() const {
    return partialVector; // return actual partialVector member, type std::vector<Partial>, const so can't be altered
};