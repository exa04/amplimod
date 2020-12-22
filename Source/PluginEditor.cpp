#include <cmath>

#include "PluginProcessor.h"
#include "PluginEditor.h"

AmpliModAudioProcessorEditor::AmpliModAudioProcessorEditor (AmpliModAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 140);

    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(0.001, 20);
    frequencySlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true);

    addAndMakeVisible(ampSlider);
    ampSlider.setRange(0, 100);
    ampSlider.setTextValueSuffix("%");
    addAndMakeVisible(ampLabel);
    ampLabel.setText("Mix", juce::dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);

    addAndMakeVisible(stereoSlider);
    stereoSlider.setRange(-50, 50);
    stereoSlider.setTextValueSuffix("%");
    addAndMakeVisible(stereoLabel);
    stereoLabel.setText("Stereo Offset", juce::dontSendNotification);
    stereoLabel.attachToComponent(&stereoSlider, true);

    AmpliModAudioProcessorEditor::setSliderStyle(frequencySlider, frequencyLabel);
    AmpliModAudioProcessorEditor::setSliderStyle(ampSlider, ampLabel);
    AmpliModAudioProcessorEditor::setSliderStyle(stereoSlider, stereoLabel);
}

AmpliModAudioProcessorEditor::~AmpliModAudioProcessorEditor()
{
}

void AmpliModAudioProcessorEditor::setSliderStyle(juce::Slider& slider, juce::Label& sliderLabel) {
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromRGB(255, 170, 220));
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(240, 140, 255));
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(220, 120, 240));
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGB(220, 120, 240));
    slider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour::fromRGB(220, 120, 240));
    slider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromRGB(90, 10, 90));
    sliderLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(90, 10, 90));
}

void AmpliModAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB(255,200,230));
    g.setColour (juce::Colour::fromRGB(90, 10, 90));
    g.setFont (15.0f);
    g.drawFittedText ("AmpMod", getLocalBounds(), juce::Justification::topLeft, 1);
}

void AmpliModAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    frequencySlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    ampSlider.setBounds(sliderLeft, 60, getWidth() - sliderLeft - 10, 20);
    stereoSlider.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 10, 20);
}
