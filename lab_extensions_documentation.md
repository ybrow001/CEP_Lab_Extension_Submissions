Yew Brown 
Student ID: 33880799
BSc Creative Computing
Creative Embedded Programming
12.12.2025 

# Supporting Documentation for Extended Lab Tasks Submission

### Introduction and General Specifications

In this document I will detail the functions of, my motivations for and possible further developments for the programmes that I have built upon. As well as give a explanation of how to run them and an of how they work.

Both accompanying projects were developed in the Bela IDE and are designed to run on a Bela, so require one to be used.
Alongside the Bela itself both require a way of hearing audio output, via an audio adapter using the 3 pin Molex-style connector on the Bela. 

## 1: Extension of Week #7, Lab #0 - Photoresistor Crossfader and Accurate Sequencing - Analog Input

### Function, Motivation and Possible Further Extensions

To extend this programme, I have created a spectral mask filter. This filter uses light intensity detected via photoresistor and input into the Bela's analogue pins to
dynamically filter audio signals, producing a noisy, granulated wet output throughout a specified frequency sprectrum. 

My motivation was to experiment with an interest way to dynamically manipulate audio, by taking a concept I had in the abstract and seeing how it would turn out in practice. 
With the intention of it being useful in developing my creative practice - especially in relation to sound production and live performance.

Some further developments I would like to implement are to: 

1. resolve the basic functional issue of component noise in the filter having amplitude that is far too high  

2. improve the functionality of the component bandpass filters (BPFs) to feature smoother attenuation and more selective frequecy ranges

The first improvement could be dealt with by further investigation into how the noise signal ends up unintentionally amplifying or otherwise becoming audible in the final output. 
A possible cause may be to do with the biquad BPFs that apply the filtering; if they cause any amplification or oscillation through resonant frequencies or similar by-product. 
This can be investigated by more deeply researching the inner working of the BPFs and biquad filter formulae, beyond simply applying them.

The second development would be impleted through introducing parameters to control the Q factor and smoothness of the BPFs, as well as to control the width of corresponding binary band gates to determine ranges where frequencies are and are not allowed to pass through the BPFs.

### Technical Explanation

- how to use
  - considerations, handling possible issues

- brief rundown of component parts, how it works, logic

## 2: Extension of Week #3, Lab #1 - Loading and Playing Samples, Using a Circular Buffer for Delay - Delay

### Function, Motivation and Possible Further Extensions

---

### Technical Explanation

- how to use
  - considerations, handling possible issues

- brief rundown of component parts, how it works, logic