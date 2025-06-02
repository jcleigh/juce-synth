#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
class SynthKnob : public juce::Slider
{
public:
    SynthKnob()
    {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        setLookAndFeel(&customLookAndFeel);
    }
    
    ~SynthKnob()
    {
        setLookAndFeel(nullptr);
    }
    
private:
    class KnobLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                             float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                             juce::Slider& slider) override
        {
            const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius;
            const float ry = centreY - radius;
            const float rw = radius * 2.0f;
            const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            
            // Fill
            g.setColour(juce::Colour(0xff2a2a2a));
            g.fillEllipse(rx, ry, rw, rw);
            
            // Outline
            g.setColour(juce::Colour(0xff505050));
            g.drawEllipse(rx, ry, rw, rw, 2.0f);
            
            // Pointer
            juce::Path p;
            const float pointerLength = radius * 0.33f;
            const float pointerThickness = 2.0f;
            p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
            p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
            
            g.setColour(juce::Colour(0xffff6600));
            g.fillPath(p);
        }
    };
    
    KnobLookAndFeel customLookAndFeel;
};

//==============================================================================
class JuceSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    JuceSynthAudioProcessorEditor(JuceSynthAudioProcessor&);
    ~JuceSynthAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JuceSynthAudioProcessor& processorRef;
    
    // UI Components
    std::unique_ptr<SynthKnob> waveformKnob;
    std::unique_ptr<SynthKnob> filterCutoffKnob;
    std::unique_ptr<SynthKnob> filterResonanceKnob;
    std::unique_ptr<SynthKnob> attackKnob;
    std::unique_ptr<SynthKnob> decayKnob;
    std::unique_ptr<SynthKnob> sustainKnob;
    std::unique_ptr<SynthKnob> releaseKnob;
    
    // Labels
    std::unique_ptr<juce::Label> waveformLabel;
    std::unique_ptr<juce::Label> filterCutoffLabel;
    std::unique_ptr<juce::Label> filterResonanceLabel;
    std::unique_ptr<juce::Label> attackLabel;
    std::unique_ptr<juce::Label> decayLabel;
    std::unique_ptr<juce::Label> sustainLabel;
    std::unique_ptr<juce::Label> releaseLabel;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    void setupKnobAndLabel(std::unique_ptr<SynthKnob>& knob, 
                          std::unique_ptr<juce::Label>& label,
                          const juce::String& labelText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceSynthAudioProcessorEditor)
};