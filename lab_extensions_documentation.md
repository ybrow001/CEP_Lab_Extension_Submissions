- Yew Brown
- Student ID: 33880799
- BSc Creative Computing
- Creative Embedded Programming
- 12.12.2025

# Supporting Documentation for Extended Lab Tasks Submission

### Introduction and General Specifications

In this document I will detail the functions of, my motivations for and possible further developments for the programmes that I have built on. As well as give an explanation of how to run them and how they work.

Both accompanying projects were developed in the Bela IDE and are designed to run on a Bela, so require one to be used.
Alongside the Bela itself, both require a way of listening to the audio output via an audio adapter using the 3 pin Molex-style connector on the Bela.

## 1: Extension of Week #7, Lab #0 - Photoresistor Crossfader and Accurate Sequencing - Analog Input

### Function, Motivation and Possible Further Extensions

To extend this programme, I have created an LDR modulated spectral mask filter. This filter utilises light intensity detected via photoresistor input into the Bela's analogue pins to dynamically filter audio signals, producing a noisy, granulated wet output throughout a specified frequency spectrum.

My motivation was to experiment with an interesting way to dynamically manipulate audio; taking an idea I had in concept and seeing how it would turn out in practice.
With the intention of it being useful in developing my own creative practice - especially in relation to sound design and live performance.

Some further developments I would like to implement are to:

1. Resolve the basic functional issue of the noise component of the filter having amplitude that is far too high. 

2. Improve the functionality of the component bandpass filters (BPFs), to feature smoother attenuation and more selective frequency ranges.

The first improvement could be dealt with by further investigation into how the noise signal ends up unintentionally amplifying or otherwise becoming audible in the final output.
A possible cause may be behaviours of the biquad BPFs that apply the filtering. That is, if they cause any amplification or oscillation through resonance or other unintended by-products.
This can be assessed by more deeply researching the inner workings of the BPFs and biquad filter formulae, beyond simply applying them.

The second development would be implemented through introducing parameters to control the Q factor and smoothness of the BPFs. As well as to control the width of corresponding binary band gates, to determine ranges where frequencies are and are not allowed to be passed through the BPFs.

### Technical Explanation

In order to use the LDR modulated filter, the Bela needs to be in a circuit containing an LDR, with the LDR outputting into the Bela's analogue pin 1. For development and testing, this was set up simply on a breadboard.

Once compiled and running, the included audio files should play.

If you cover the LDR to dim the light and shine a brighter light source on it, it should mix between the two audio files.

As it does this, the filter's noise parameters should also change. This should be audible as more fine grained noise when the LDR detects a higher intensity of light and more sparse, harsh grained noise when it detects low light.

Due the light in each space this programme may be run being an uncontrolled variable, the mixing of the audio files and parameters of the filter can vary in their balance. In order to solve any issues with hearing the effects clearly, the global variable called **lightThreshold**  can be adjusted from between **0.0** and **1.0** to alter the balance of the mix and find a sweetspot, where the outcome is clearly audible.

The project folder contains a main render.cpp file which handles the main running of the code, via a render loop iterating at the audio block size and nested loop at the sample rate derived from the Bela's context.

It also contains five classes: Clamping, NoiseGenerator, BandGate, BandpassFilter and SpectralMask.

The Clamping class is from the initial lab task, and limits its input to a new specified range.

The following four classes are all interconnected in creating the resulting filter, and are utilised by SpectralMask to build it.

NoiseGenerator is run to create a sparse noise signal, used as the means of triggering the band gates and creating the texture of the filter.

BandGate is used to create a series of binary frequency band gates in a specified number of divisions over the given range. The instances of this class (in this case, variables created from a struct) are created inside the SpectralMask class.

BandpassFilter is used to create the filters that allow frequencies to either pass audibly or be muted. Its frequency bands are created through the same process as the BandGates and instances of both have a one to one correspondence in size and frequency range.

SpectralMask is used to create instances of and assemble the other classes to make the complete filter functionality. It also includes a process method, which applies the BandpassFilters to the dry signal to produce the filtered output.

In the render.cpp file, instances of the NoiseGenerator and SpectralMask classes are constructed using a pointer in a global variable, then initialized and passed values in the Bela's setup() function.

In the render() loop of the render.cpp file, the values from the LDR are received and clamped, the samples are played and updated at the sample rate and the filter class instances are updated and passed the required values as arguments. The NoiseGenerator object is passed a partial frequency randomisation probability scaled by the LDR value. The SpectralMask object is passed the dry signal as well as other necessary values for processing, producing the filtered output.

At the bottom of the render() loop, some safety precautions are taken, such as clipping and mixing the dry and wet signals to ensure a safe and listenable output.
This was especially important, as in its current state, the noise signal inside of the filter is far too high in amplitude and is necessary to limit.

The final signal is then passed to the Bela's audioWrite() function.

## 2: Extension of Week #3, Lab #1 - Loading and Playing Samples, Using a Circular Buffer for Delay - Delay

### Function, Motivation and Possible Further Extensions

To extend the second programme, I added more parameters typical of a digital IIR delay and a GUI to control them. These include a delay time control in seconds, a feedback control and wet and dry levels. The GUI utilises simple sliders.

The motivation was to try my hand at more conventional DSP work by attempting to create a more standard, functional delay and to understand the process of doing so better. Although this kind of delay is commonplace, it will still be very useful for my own practice - especially for use in realtime as I begin to explore DSP and embedded programming more in live sound creation, sound design and computing.

A couple of further developments that could be implemented are to:

1. Add novel functionality, beyond recreating standard features of a digital delay.

2. Improve the design for cleaner, more usable audio output.

For the first improvement, this can be achieved by experimenting with it and simply applying creative ideas that I would like to utilise for myself.
For example, a further but less original feature that I was planning to add is a "colour" to the delayed signal, by dynamically filtering over it time.
Another example would be to add harmonisation and pitch detection. Which could also be a way to explore my interest in microtonal harmony and just intonation in a DSP context.

The second development continues the less creative, more technical approach. In which I would like to research how to reduce the clicking and popping produced by altering the delay time. I have tried to do this by smoothing and interpolating between delayed samples, but it is still present. I am not exactly sure what causes it, so further research and exploration is needed to solve it.

### Technical Explanation

To use this delay programme, it simply needs the Bela connection, to be run and compiled and for the GUI to be opened.

Once it is running the default audio file should play. Adjusting the sliders of the GUI should change the delay parameters and how it sounds.

The project includes a main render.cpp file, and also contains a Delay class made up of a header and source file, which is called on in the render loop of the render.cpp file.

The Delay class handles the processing on the input signal to produce a delayed output.

Inside its process method it first limits the input wet and dry gains, feedback amount and delay time to prevent error and unintended effects. It then sets up the distance in samples of the delay read pointer behind the delay write pointer, which read and write from/to the delay buffer respectively, feeds the input amount of wet signal back into the dry signal to create feedback, then applies the wet and dry gains to both corresponding signals before outputting the sum of them.

The values of the GUI sliders are got using the methods of the GUI classes, which are included from the Bela Library tab in render.cpp. In the render loop, the GUI slider values are then assigned to variables and passed into delay.process() as arguments to control the delay.

The processed signal is finally output using the audioWrite() function.

## References

DSP Cookbook - Robert Bristow-Johnson, Raymond Toy:
- https://www.w3.org/TR/audio-eq-cookbook/#formulae
- Used as reference for the Biquad BPF (constant 0db peak) formula.