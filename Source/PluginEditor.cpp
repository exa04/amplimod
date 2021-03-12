#include <cmath>

#include "PluginProcessor.h"
#include "PluginEditor.h"

AmpliModAudioProcessorEditor::AmpliModAudioProcessorEditor (AmpliModAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    juce::OwnedArray <juce::Slider> sliders;

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 145, 191));

    colors.apply(getLookAndFeel(), 200);
    audioProcessor.visualizer.setColours(colors.shades[2], colors.shades[0]);

    createSlider(frequencySlider, frequencyLabel, 0.001, 20, " Hz", "Frequency");
    createSlider(ampSlider, ampLabel, 0, 100, "%", "Mix");
    createSlider(stereoSlider, stereoLabel, -50, 50, "%", "Stereo Offset");

    bounds = components.bounds(3);

    setSize(components.setWidth(600), components.setHeightFromSliders(3));

    addAndMakeVisible(simplifyBtn);
    simplifyBtn.setClickingTogglesState(true);

    addAndMakeVisible(hfBtn);
    hfBtn.setClickingTogglesState(true);
    hfBtn.onStateChange = [this]() {
        if (hfBtn.getToggleState()) {
            frequencySlider.setRange(0.001, 1000);
            repaint();
        }
        else {
            frequencySlider.setRange(0.001, 20);
            repaint();
        }
        audioProcessor.mHF.set(hfBtn.getToggleState());
    };

    if (!audioProcessor.mHF.get()) {
        hfBtn.setToggleState(false, juce::NotificationType::sendNotification);
    }
    else {
        hfBtn.setToggleState(true, juce::NotificationType::sendNotification);
    }
    int v = audioProcessor.mHF.get() ? 1 : 0;

    addAndMakeVisible(components.footerTxt);
    addAndMakeVisible(audioProcessor.visualizer);
}

AmpliModAudioProcessorEditor::~AmpliModAudioProcessorEditor()
{
}

void AmpliModAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AmpliModAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    auto topOffset = 130;

    /*
    simplifyBtn.setBounds       (getWidth() - 50, 120 + topOffset, 40, 20);
    hfBtn.setBounds             (getWidth() - 50, 160 + topOffset, 40, 20);
    */

    frequencySlider.setBounds(bounds[0]);
    ampSlider.setBounds(bounds[1]);
    stereoSlider.setBounds(bounds[2]);

    audioProcessor.visualizer.setBounds(10, 10, getWidth() - 20, 110);

    components.setFooter("AmpliMod", colors, getLocalBounds());
}

void AmpliModAudioProcessorEditor::createSlider(juce::Slider& slider, juce::Label& label, double rstart, double rend, std::string suffix, std::string name) {
    addAndMakeVisible(slider);
    slider.setRange(rstart, rend);
    slider.setTextValueSuffix(suffix);
    slider.setColour(juce::Slider::textBoxOutlineColourId, getLookAndFeel().findColour(juce::Slider::textBoxOutlineColourId));
    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);

    label.attachToComponent(&slider, true);
}