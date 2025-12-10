/*
- spectral mask filter -

features:
- noise generator
- binary gates
- bandpass filter
- spectral mask processor

how to use: 
- set up the Bela to receive data from a Light-Dependent Resistor (LDR)

- when run, if the LDR is exposed to more or less light -
  it should mix between two sound files and alter the parameters of the filter.
  
- the dry (original) and wet (filtered) signals can be mixed manually -
  by altering the multiplier in the variables dryOut and wetOut at the end of the render() loop -
  (within the range 0.0 - 1.0)
  
:)
*/

#include <Bela.h> // automatically includes standard library
#include <libraries/AudioFile/AudioFile.h>
#include <cmath>
#include "Clamping.h"
#include "NoiseGenerator.h"
#include "BandGate.h"
#include "BandpassFilter.h"
#include "SpectralMask.h"

/* global variables */

float sampleRate;

/* LDR */

const int ldrPin = 1;
float lightThreshold = 0.667f;

/* pointers to class instances  */

SpectralMask* spectralMask;
NoiseGenerator* noise;

/* audio sampling */

//file paths for audio
std::string daytime = "9zpi_pad.wav";
std::string nighttime = "15zpi_swirly.wav";

//buffer and read pointer for audio
std::vector<std::vector<float>> dayBuffer;
unsigned int dayReadPointer = 0;

std::vector<std::vector<float>> nightBuffer;
unsigned int nightReadPointer = 0;

/* setup */

bool setup(BelaContext* context, void* userData) {
	srand(time(NULL));
	
	sampleRate = context->audioSampleRate;
	
	spectralMask = new SpectralMask(20.0f, 20000.0f, sampleRate, 5); 
	// args: spectrumBottom, spectrumTop, sample rate, bands per octave
	
	noise = new NoiseGenerator(20.0f, 20000.0f, sampleRate, 5); 
	// args: freqRangeLow, freqRangeHigh, sample rate, number of partials

    // load audio file
    dayBuffer = AudioFileUtilities::load(daytime);
    nightBuffer = AudioFileUtilities::load(nighttime);

    return true;
}

void render(BelaContext* context, void* userData) {
	
    for (unsigned int n = 0; n < context->audioFrames; n++) {
    	
    	/* LDR */
    	
    	float ldrValue = analogRead(context, n/context->audioFrames, ldrPin);
    	
    	float normalisedldr = ldrValue / lightThreshold;
    	normalisedldr = clamp(normalisedldr,0.0f,1.0f);
    	
    	float gain1 = normalisedldr;
    	float gain2 = (normalisedldr * -1) + 1;
    	
    	/* samples */
    	
        //fetch samples from buffer
        float source1 = 0.0f;
        float source2 = 0.0f;

        if (dayReadPointer < dayBuffer[0].size())
            source1 = dayBuffer[0][dayReadPointer++];

        //looping audio files
        if (dayReadPointer >= dayBuffer[0].size())
            dayReadPointer = 0;
       
        if (nightReadPointer < nightBuffer[0].size())
            source2 = nightBuffer[0][nightReadPointer++];
       
        if (nightReadPointer >= nightBuffer[0].size())
            nightReadPointer = 0;
        
        //output source
        float out1 = source1 * gain1;
        float out2 = source2 * gain2;
        
        /* spectral mask filter */
        
        // reset band gates' active state
        spectralMask->reset();
        
        // randomise freq, amp and update phase
        noise->updatePartials(0.5f, normalisedldr); // args: randomisationProbability, normalised LDR value
        
        for(auto &p : noise->getPartials()) { // loop over partials in noise member partialVector
    		// pass each partial's frequency as arg
        	spectralMask->checkActivate(p.freq); // arg: partial frequency, for each partial
        };
        
        /* filtering dry signal and post-processing */
        
        float clippedDry = clamp(out1 + out2, 0.0f, 1.0f);
        
        float filteredOut = spectralMask->process(clippedDry, 0.33f, 0.0f); // arg: drySignal
        
        // !! wet signal currently very hot! needs clipping for safety !!
        float clippedWet = clamp(filteredOut, 0.0f, 1.0f); // produces distortion
        
        // apply gain to dry and wet signals
        float dryOut = clippedDry * 0.5; 
        float wetOut = clippedWet * 0.01;
        
        /* output */
        
        for (unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
            audioWrite(context, n, channel, dryOut + wetOut);
        }
    }
};

void cleanup(BelaContext* context, void* userData) {};
