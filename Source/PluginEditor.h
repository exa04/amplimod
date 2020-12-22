#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AmpliModAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AmpliModAudioProcessorEditor (AmpliModAudioProcessor&);
    ~AmpliModAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AmpliModAudioProcessor& audioProcessor;
    juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider ampSlider;
    juce::Label  ampLabel;
    juce::Slider stereoSlider;
    juce::Label  stereoLabel;

    void setSliderStyle(juce::Slider& slider, juce::Label& sliderLabel);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpliModAudioProcessorEditor)

    juce::AudioProcessorValueTreeState::SliderAttachment mFreqAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramFreq, frequencySlider };
    juce::AudioProcessorValueTreeState::SliderAttachment mMixAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramMix, ampSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment mStereoOffsetAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramStereoOffset, stereoSlider };
};
