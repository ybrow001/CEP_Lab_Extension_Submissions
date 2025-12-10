#pragma once
#include "BandGate.h"
#include "BandpassFilter.h"
#include <vector>
#include <cmath>
#include <iostream>

class SpectralMask {
	public:
		SpectralMask(float spectrumBottom, float spectrumTop, int sampleRate, int bandsPerOctave); 
		// contructor, creates instances of BandGate and BPF, populates vectors with them
		
		void reset(); // reset band active state to false
		void checkActivate(float partialFreq); // activate band if noise partial detected 
		float process(float drySignal, float dryGain, float wetGain); // apply BPF if band active
	private: 
		int bandsPerOctave;
		float spectrumBottom;
		std::vector<BandGate> bandVector;
		std::vector<BandpassFilter> bpfVector;
};