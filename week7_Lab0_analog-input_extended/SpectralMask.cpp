#include "SpectralMask.h"
#include "BandGate.h"
#include "BandpassFilter.h"
#include <vector>
#include <cmath>
#include <iostream>

SpectralMask::SpectralMask(float spectrumBottom, float spectrumTop, int sampleRate, int bandsPerOctave) 
: bandsPerOctave(bandsPerOctave), spectrumBottom(spectrumBottom)
{
	for(unsigned int f = spectrumBottom; f < spectrumTop;f *= 2) { // frequency by octave for given spectrum
		for(unsigned int n = 0; n < bandsPerOctave; n++) { // freq by 2 steps of given EDO
		
			float lowLimit = f * pow(2,n/bandsPerOctave); // in n/x, x = no. to divide octave by, n = step
			float highLimit = f * pow(2,(n+1)/bandsPerOctave);
			
			float bandwidth = highLimit - lowLimit; // add divisor or multiplier to adjusts bandsize "(hiLim - loLim)/div OR mult"
			float centreFreq = lowLimit + (bandwidth / 2);
			
			bandVector.push_back(initialiseBandGate(lowLimit, highLimit));
			
			bpfVector.emplace_back(centreFreq, bandwidth, sampleRate); // create instance of BandpassFilter inside bpf vector
		};
	};
	// std::cout << bandVector.size();
	// std::cout << bpfVector.size();
};

void SpectralMask::reset() {
	for(auto &b : bandVector) { // reset each freq band state to false
        	b.active = false;
    }
};

void SpectralMask::checkActivate(float partialFreq) {
	int bandIndex = static_cast<int>(floor(bandsPerOctave * log2(partialFreq/spectrumBottom))); // find BandGate index based on freq
	
	if(bandIndex >= 0 && bandIndex < (int)bandVector.size()) // check if BandGate is within useable range and activate
		bandVector[bandIndex].active = true;
};

float SpectralMask::process(float drySignal, float dryGain, float wetGain) {
	float filteredOutput = 0.0f;
	
	int activeBands = 0;
	
	for(unsigned int i = 0; i < bandVector.size(); i++) {
		bpfVector[i].reset();
        if(bandVector[i].active == true) {
        	filteredOutput += 0.5 * bpfVector[i].process(drySignal);
        	activeBands++;
        }
    };
  
    if(activeBands > 0)
    	filteredOutput /= activeBands;
    
    return filteredOutput;
};
