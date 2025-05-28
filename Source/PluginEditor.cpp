#include "PluginProcessor.h"
#include "PluginEditor.h"

JuceSynthAudioProcessorEditor::JuceSynthAudioProcessorEditor(JuceSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Set the size of the editor window
    setSize(400, 300);
}

JuceSynthAudioProcessorEditor::~JuceSynthAudioProcessorEditor()
{
}

void JuceSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill the background
    g.fillAll(juce::Colours::darkgrey);
    
    // Set text color and font
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    
    // Display a welcome message
    g.drawFittedText("Hello World Synthesizer", getLocalBounds(), juce::Justification::centred, 1);
    g.setFont(12.0f);
    g.drawFittedText("Play MIDI notes to hear a sine wave", 
                     juce::Rectangle<int>(0, getHeight() - 30, getWidth(), 20), 
                     juce::Justification::centred, 1);
}

void JuceSynthAudioProcessorEditor::resized()
{
    // This is called when the editor is resized.
    // If you add any components to your editor, you should update their positions here.
}