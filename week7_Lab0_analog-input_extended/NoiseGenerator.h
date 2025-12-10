#pragma once
#include <cmath>
#include <cstdlib>
#include <vector>

class NoiseGenerator {
	public: 
		struct Partial {
			float freq; 
			float phase; 
			float amp;
		};
		NoiseGenerator(float freqRangeLowIn, float freqRangeHighIn, int sampleRateIn, int numPartials); // constructor, initialise Partial variables
		void updatePartials(float randomProbability, float normalisedLDRValue); // update frequency, amp and phase
		const std::vector<Partial>& getPartials() const;
	private:
		std::vector<Partial> partialVector;
		int sampleRate;
		float freqRangeLow;
		float freqRangeHigh;
};