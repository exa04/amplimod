#pragma once

#include <JuceHeader.h>

class Visualizer : public juce::AudioVisualiserComponent
{
public:
    Visualizer() : AudioVisualiserComponent(2)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
    }
};

//==============================================================================
/**
*/
class AmpliModAudioProcessor  : public juce::AudioProcessor,
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    AmpliModAudioProcessor();
    ~AmpliModAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void parameterChanged(const juce::String& parameterID, float newValue) override;
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

    float phase_delta;
    float fs;

    juce::AudioProcessorValueTreeState& getValueTreeState();

    static juce::String paramFreq;
    static juce::String paramMix;
    static juce::String paramStereoOffset;
    static juce::String paramSimplify;
    static juce::String paramHF;

    juce::Atomic<float>   mFreq{ 1.0f };
    juce::Atomic<float>   mMix{ 100.0f };
    juce::Atomic<float>   mStereoOffset{ 0.0f };
    juce::Atomic<bool>    mSimplify{ false };
    juce::Atomic<float>   mHF{ true };

    Visualizer visualizer;

    bool simplified = true;
private:

    juce::UndoManager                  mUndoManager;
    juce::AudioProcessorValueTreeState mState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpliModAudioProcessor)
};
