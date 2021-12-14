/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Resonator2poleAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener
{
public:
    Resonator2poleAudioProcessorEditor (Resonator2poleAudioProcessor&);
    ~Resonator2poleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //==============================================================================
    void sliderValueChanged (juce::Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Resonator2poleAudioProcessor& audioProcessor;
    
    juce::Slider cutSlider;
    juce::Label cutLabel;
    
    juce::Slider resSlider;
    juce::Label resLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Resonator2poleAudioProcessorEditor)
};


