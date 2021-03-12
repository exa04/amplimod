#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"


class Visuals {
public:
	class Colors {
	private:
		float degToFloat(float deg) {
			return deg / 360;
		}
	public:
		float hue = 0.93;

		juce::Colour bg = juce::Colour::fromRGB(232, 233, 243);
		juce::Colour fg = juce::Colour::fromRGB(6, 6, 12);
		std::vector<juce::Colour> shades = {
			juce::Colour::fromHSL(hue, 1., 0.78, 1.),
			juce::Colour::fromHSL(hue - degToFloat(6), 0.71, 0.85, 1.),
			juce::Colour::fromHSL(hue - degToFloat(17), 0.44, 0.88, 1.),
		};

		void apply(juce::LookAndFeel& LookAndFeel, int newHue = 335) {
			this->hue = degToFloat(newHue);

			this->shades = {
				juce::Colour::fromHSL(hue, 1., 0.78, 1.),
				juce::Colour::fromHSL(hue - degToFloat(6), 0.71, 0.85, 1.),
				juce::Colour::fromHSL(hue - degToFloat(17), 0.44, 0.88, 1.),
			};

			LookAndFeel.setColour(juce::Slider::thumbColourId, shades[0]);
			LookAndFeel.setColour(juce::Slider::trackColourId, shades[1]);
			LookAndFeel.setColour(juce::Slider::backgroundColourId, shades[2]);

			LookAndFeel.setColour(juce::Slider::textBoxBackgroundColourId, shades[2]);
			LookAndFeel.setColour(juce::Slider::textBoxOutlineColourId, shades[0]);
			LookAndFeel.setColour(juce::Slider::textBoxTextColourId, fg);
			LookAndFeel.setColour(juce::Slider::textBoxHighlightColourId, shades[0]);

			LookAndFeel.setColour(juce::Label::textColourId, fg);

			LookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, this->bg);
		}
	};
	
	class Components {
	private:
		int width = 600;
		int height = 0;
	public:
		juce::Label footerTxt;
		int setWidth(int nWidth) {
			this->width = nWidth;
			return this->width;
		}
		int getWidth() {
			return this->width;
		}
		int setHeight(int nHeight) {
			this->height = nHeight;
			return this->height;
		}
		int setHeightFromSliders(int sliders) {
			this->height = sliders * 40 + 140;
			return this->height;
		}
		int getHeight(std::vector<juce::Slider> sliders = {}) {
			return this->height;
		}
		void setFooter(juce::String name, Visuals::Colors colors, juce::Rectangle<int> area) {
			footerTxt.setColour(juce::Label::ColourIds::backgroundColourId, colors.shades[0]);
			footerTxt.setText(name + " by ari", juce::dontSendNotification);
			footerTxt.setBounds(area.removeFromBottom(20));
		}
		std::vector<juce::Rectangle<int>> bounds(int sliderCount) {
			std::vector<juce::Rectangle<int>> bounds;
			bounds.resize(sliderCount);
			for (int i = 0; i < sliderCount; i++) {
				juce::Rectangle<int> boundBox = { 120, i*40 + 130, width - 120 - 10, 20 };
				bounds[i] = boundBox;
			}
			return bounds;
		}
	};
};