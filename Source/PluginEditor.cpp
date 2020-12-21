#define _USE_MATH_DEFINES

#include <cmath>

#include "PluginProcessor.h"
#include "PluginEditor.h"

AmpliModAudioProcessorEditor::AmpliModAudioProcessorEditor (AmpliModAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 80);
    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(1, 20);
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
    ampSlider.onValueChange = [this] { audioProcessor.sin_amp = ampSlider.getValue() / 100; };
    frequencySlider.onValueChange = [this] { audioProcessor.phase_delta = 2 * M_PI * frequencySlider.getValue() / audioProcessor.fs; DBG(2 * M_PI * frequencySlider.getValue() / audioProcessor.fs); };
    frequencySlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromRGB(255, 170, 220));
    frequencySlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(240, 140, 255));
    frequencySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(220, 120, 240));
    frequencySlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGB(220, 120, 240));
    frequencySlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour::fromRGB(220, 120, 240));
    frequencySlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromRGB(90, 10, 90));
    frequencyLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(90, 10, 90));

    ampSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromRGB(255, 170, 220));
    ampSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(240, 140, 255));
    ampSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(220, 120, 240));
    ampSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGB(220, 120, 240));
    ampSlider.setColour(juce::Slider::ColourIds::textBoxHighlightColourId, juce::Colour::fromRGB(220, 120, 240));
    ampSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour::fromRGB(90, 10, 90));
    ampLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(90, 10, 90));
}

AmpliModAudioProcessorEditor::~AmpliModAudioProcessorEditor()
{
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
}
