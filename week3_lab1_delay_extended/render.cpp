/*
-- simple multi-parameter IIR digital delay --

features: 

- time in seconds from 0.1 to delay buffer size in seconds
- feedback in percentage of range 0.0 to 0.95
- dry signal output gain 0.0 - 1.0
- wet signal output gain 0.0 - 1.0

to use: 

open gui ("Launch GUI" sliders button in bela.local IDE) and adjust the sliders to change the delay parameters!

use your own audio by adding a .wav file to the project folder and altering the sample file path:
- std::string sample = "yourFile.wav";

:)
*/

#include <Bela.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>
#include "Delay.h" // including the header file of my Delay class

// instance of Delay class
Delay delay; 

// instances of Gui and GuiController
Gui gui;
GuiController guiController;

//file path for sample
std::string sample = "9zpi_runs.wav";

//source file buffer
std::vector<std::vector<float> > sampleData;

//position of last read sample from file
int samplePlayhead; 

// delay buffer size in s, max delay length
float delayBufferSeconds; 
int sampleRate;

bool setup(BelaContext *context, void *userData) {
	
	sampleRate = context->audioSampleRate;
	
	sampleData = AudioFileUtilities::load(sample);
	samplePlayhead = 0;
	
	delayBufferSeconds = 0.975f;
	
	delay.init(sampleRate, delayBufferSeconds);
	
	gui.setup(context->projectName);
	guiController.setup(&gui, "delay parameters");
	guiController.addSlider("delay time (s)", 0.33f, 0.1f, delayBufferSeconds);
	guiController.addSlider("feedback", 0.33f, 0.0f, 0.95f);
	guiController.addSlider("dry gain", 0.25f, 0.0f, 1.0f);
	guiController.addSlider("wet gain", 0.125f, 0.0f, 1.0f);
	
	return true;
}

void render(BelaContext *context, void *userData) {
	
    for(unsigned int n = 0; n < context->audioFrames; n++) { 
        // increment read pointer and reset to 0 when end of file is reached
        if(samplePlayhead >= sampleData[0].size())
            samplePlayhead = 0;
        
        samplePlayhead += 1;
        
        for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) { // loop over channels
        
        	// assign delay.process() argument values to variables for readability 
        	float drySignal = sampleData[channel % sampleData.size()][samplePlayhead];
    		
    		float delayTimeSeconds = guiController.getSliderValue(0);
    		float feedbackAmount = guiController.getSliderValue(1);
    		float dryGain = guiController.getSliderValue(2);
    		float wetGain = guiController.getSliderValue(3);
    		
    		// pass args: delay.process(input, delayTimeSeconds, feedback, dryGain, wetGain) 
    		float delayOut = delay.process(drySignal, delayTimeSeconds, feedbackAmount, dryGain, wetGain); 
    		
    		audioWrite(context, n, 0, delayOut); // left out
    		audioWrite(context, n, 1, delayOut); // right out
    	}
    }
};

void cleanup(BelaContext *context, void *userData) {
};
