/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <stdio.h>

//==============================================================================
/**
*/
class Resonator2poleAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Resonator2poleAudioProcessor();
    ~Resonator2poleAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    //Setters    
    void setCut(float x){
        cut = x;
        updateFilter();
    };
    void setRes(float x){
        res = x;
        updateFilter();
    };
    void updateFilter(){
        // Resonator filter formulas from the book (chapter 6.3.1)
        frec = juce::MathConstants<float>::twoPi * cut / currentSampleRate;
        bw = cut / res;
        b2L = exp(- juce::MathConstants<float>::twoPi * bw / currentSampleRate);
        b2R = b2L;
        b1L = (-4 * b2L / (1+b2L)) * cos(frec);
        b1R = b1L;
        a0L = (1-b2L) * sqrt(1 - b1L * b1L / (4 * b2L));
        a0R = a0L;
    };
    
    //getters (for gui)
    float getRes(){
        return res;
    };
    float getCut(){
        return cut;
    };

private:
    
    //filter coefficents
    float b2L, b2R, b1L,b1R, a0L, a0R; //filter coefficients
    float z1L, z1R, z2L, z2R; //delays
    float cut, res; //cutoff frequency, resonance (Q)
    float frec; //normalized cutoff frequency
    float bw; //bandwith
    float currentSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resonator2poleAudioProcessor)
};



