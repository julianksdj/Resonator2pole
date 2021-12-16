/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Resonator2poleAudioProcessorEditor::Resonator2poleAudioProcessorEditor (Resonator2poleAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
    addAndMakeVisible(cutSlider);
    cutSlider.setRange (20.0, 20000.0);
    cutSlider.addListener (this);
    cutSlider.setValue(audioProcessor.getCut());
    addAndMakeVisible (cutLabel);
    cutLabel.setText ("Cutoff frequency", juce::dontSendNotification);
    cutLabel.attachToComponent (&cutSlider, true);
    
    addAndMakeVisible(resSlider);
    resSlider.setRange (0.5,10.0);
    resSlider.addListener (this);
    resSlider.setValue(audioProcessor.getRes());
    addAndMakeVisible (resLabel);
    resLabel.setText ("Resonance", juce::dontSendNotification);
    resLabel.attachToComponent (&resSlider, true);
}

Resonator2poleAudioProcessorEditor::~Resonator2poleAudioProcessorEditor()
{
}

//==============================================================================
void Resonator2poleAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (19.0f);
    g.drawFittedText ("Resonator 2-pole filter", 0, 30, getWidth(), 5, juce::Justification::centred, 1);
    //g.setFont (15.0f);
    //g.drawFittedText ("y(n) = a0 * x(n) - b1 * y(n-1)", 0, 60, getWidth(), 5, juce::Justification::centred, 1);
}

void Resonator2poleAudioProcessorEditor::resized()
{
    cutSlider.setBounds(100,140,250,20);
    resSlider.setBounds(100,160,250,20);
}


void Resonator2poleAudioProcessorEditor::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &cutSlider)
    {
        audioProcessor.setCut(cutSlider.getValue());
    }
    if (slider == &resSlider)
    {
        audioProcessor.setRes(resSlider.getValue());
    }
    
}


