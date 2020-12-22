#define _USE_MATH_DEFINES

#include <cmath>

#include "PluginProcessor.h"
#include "PluginEditor.h"

float phase_index[] = { 0, 0 };

juce::String AmpliModAudioProcessor::paramFreq("Frequency");
juce::String AmpliModAudioProcessor::paramMix("Mix");
juce::String AmpliModAudioProcessor::paramStereoOffset("Stereo Offset");

//==============================================================================
AmpliModAudioProcessor::AmpliModAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    mState(*this, &mUndoManager, "AmpliMod",
        {
        std::make_unique<juce::AudioParameterFloat>(paramFreq,
            TRANS("Frequency"),
            juce::NormalisableRange<float>(0.001f, 20.0f, 0.001),
            mFreq.get(), "hz",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(paramMix,
            TRANS("Mix"),
            juce::NormalisableRange<float>(0.0f, 100.0f, 0.001),
            mMix.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(paramStereoOffset,
            TRANS("Stereo Offset"),
            juce::NormalisableRange<float>(-50.0f, 50.0f, 0.001),
            mStereoOffset.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),
        })
{
    mState.addParameterListener(paramFreq, this);
    mState.addParameterListener(paramMix, this);
    mState.addParameterListener(paramStereoOffset, this);
}

void AmpliModAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == paramMix) {
        mMix = newValue;
    }
    else if (parameterID == paramFreq) {
        mFreq = newValue;
    }
    else if (parameterID == paramStereoOffset) {
        mStereoOffset = newValue;
    }
}

AmpliModAudioProcessor::~AmpliModAudioProcessor()
{
}

//==============================================================================
const juce::String AmpliModAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmpliModAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmpliModAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AmpliModAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AmpliModAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmpliModAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmpliModAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmpliModAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AmpliModAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmpliModAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AmpliModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = sampleRate;
    phase_delta = 2 * M_PI / fs;
}

void AmpliModAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmpliModAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void AmpliModAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    double sin_amp = mMix.get() / 100;
    phase_delta = 2 * M_PI * mFreq.get() / fs;
    float phase_offset = 2 * M_PI * mStereoOffset.get() / 100;
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i){
            if (phase_index[channel] > 2 * M_PI) { phase_index[channel] = 0; };
            phase_index[channel] += phase_delta;
            float sine;

            if(channel == 0)
                sine = sin(phase_index[channel]);
            else
                sine = sin(phase_index[channel] + phase_offset);

            channelData[i] *= 1 - (sine * sin_amp);
        }
        DBG(sin_amp);
     
    }
}

//==============================================================================
bool AmpliModAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AmpliModAudioProcessor::createEditor()
{
    return new AmpliModAudioProcessorEditor (*this);
}

//==============================================================================
void AmpliModAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    mState.state.writeToStream(stream);
}

juce::AudioProcessorValueTreeState& AmpliModAudioProcessor::getValueTreeState()
{
    return mState;
}

void AmpliModAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        mState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmpliModAudioProcessor();
}
