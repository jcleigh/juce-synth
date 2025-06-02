#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>
#include "PluginProcessor.h"

class JuceSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    JuceSynthAudioProcessorEditor(JuceSynthAudioProcessor&);
    ~JuceSynthAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JuceSynthAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceSynthAudioProcessorEditor)
};