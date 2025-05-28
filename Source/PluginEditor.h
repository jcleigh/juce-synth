#pragma once

#include <JuceHeader.h>
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