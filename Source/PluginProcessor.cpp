/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Resonator2poleAudioProcessor::Resonator2poleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Resonator2poleAudioProcessor::~Resonator2poleAudioProcessor()
{
}

//==============================================================================
const juce::String Resonator2poleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Resonator2poleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Resonator2poleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Resonator2poleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Resonator2poleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Resonator2poleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Resonator2poleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Resonator2poleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Resonator2poleAudioProcessor::getProgramName (int index)
{
    return {};
}

void Resonator2poleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Resonator2poleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    currentSampleRate = sampleRate;
    z1L = 0.0;
    z1R = 0.0;
    z2L = 0.0;
    z2R = 0.0;
    setCut(500.0);
    setRes(0.5);
    
}

void Resonator2poleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Resonator2poleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Resonator2poleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //LEFT CHANNEL PROCESSING
    int channel = 0;
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto* channelData = buffer.getWritePointer (channel);
        // Input sample is x(n)
        float xn = channelData[sample];
        // READ: Delay samples y(n-1), y(n-2)
        float yn_2 = z2L;
        float yn_1 = z1L;
        // Difference Equation
        float yn = a0L * xn - b1L * yn_1 - b2L * yn_2;
        // WRITE: Delay with current y(n)
        z2L = z1L;
        z1L = yn;
        // Output sample is y(n)
        channelData[sample] = yn;
    }
    //RIGHT CHANNEL PROCESSING
    channel = 1; //right channel
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto* channelData = buffer.getWritePointer (channel);
        // Input sample is x(n)
        float xn = channelData[sample];
        // READ: Delay samples y(n-1), y(n-2)
        float yn_2 = z2R;
        float yn_1 = z1R;
        // Difference Equation
        float yn = a0R * xn - b1R * yn_1 - b2R * yn_2;
        // WRITE: Delay with current y(n)
        z2R = z1R;
        z1R = yn;
        // Output sample is y(n)
        channelData[sample] = yn;
    }
}

//==============================================================================
bool Resonator2poleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Resonator2poleAudioProcessor::createEditor()
{
    return new Resonator2poleAudioProcessorEditor (*this);
}

//==============================================================================
void Resonator2poleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Resonator2poleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Resonator2poleAudioProcessor();
}

