#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Colors.h"

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
    Visuals::Colors colors;
    Visuals::Components components;

    std::vector<juce::Rectangle<int>> bounds;

    void createSlider(juce::Slider& slider, juce::Label& label, double rstart, double rend, std::string suffix, std::string name);
    
    juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider ampSlider;
    juce::Label  ampLabel;
    juce::Slider stereoSlider;
    juce::Label  stereoLabel;

    juce::TextButton simplifyBtn{ "ECO" };
    juce::TextButton hfBtn{ "HF" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpliModAudioProcessorEditor)

    juce::AudioProcessorValueTreeState::SliderAttachment mFreqAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramFreq, frequencySlider };
    juce::AudioProcessorValueTreeState::SliderAttachment mMixAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramMix, ampSlider };
    juce::AudioProcessorValueTreeState::SliderAttachment mStereoOffsetAttachment{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramStereoOffset, stereoSlider };
    juce::AudioProcessorValueTreeState::ButtonAttachment mHF{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramHF, hfBtn };
    juce::AudioProcessorValueTreeState::ButtonAttachment mSimplify{ audioProcessor.getValueTreeState(), AmpliModAudioProcessor::paramSimplify, simplifyBtn };
};
