#include "PluginProcessor.h"
#include "PluginEditor.h"

JuceSynthAudioProcessorEditor::JuceSynthAudioProcessorEditor(JuceSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Set the size of the editor window to a 90s synth style
    setSize(700, 400);
    
    // Setup oscillator section
    setupKnobAndLabel(waveformKnob, waveformLabel, "WAVEFORM");
    waveformKnob->setRange(0, 4, 1);
    waveformKnob->setTextValueSuffix("");
    waveformKnob->textFromValueFunction = [](double value) {
        const char* names[] = {"Sine", "Saw", "Square", "Triangle", "Noise"};
        return juce::String(names[static_cast<int>(value)]);
    };
    
    // Setup filter section
    setupKnobAndLabel(filterCutoffKnob, filterCutoffLabel, "CUTOFF");
    setupKnobAndLabel(filterResonanceKnob, filterResonanceLabel, "RESONANCE");
    
    // Setup envelope section
    setupKnobAndLabel(attackKnob, attackLabel, "ATTACK");
    setupKnobAndLabel(decayKnob, decayLabel, "DECAY");
    setupKnobAndLabel(sustainKnob, sustainLabel, "SUSTAIN");
    setupKnobAndLabel(releaseKnob, releaseLabel, "RELEASE");
    
    // Create parameter attachments
    auto& params = processorRef.getValueTreeState();
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "waveform", *waveformKnob);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "filterCutoff", *filterCutoffKnob);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "filterResonance", *filterResonanceKnob);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "attack", *attackKnob);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "decay", *decayKnob);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "sustain", *sustainKnob);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "release", *releaseKnob);
}

JuceSynthAudioProcessorEditor::~JuceSynthAudioProcessorEditor()
{
}

void JuceSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    // 90s hardware synth style background
    juce::Colour backgroundMain(0xff1a1a1a);
    juce::Colour backgroundAccent(0xff2a2a2a);
    juce::Colour orange(0xffff6600);
    
    // Fill main background
    g.fillAll(backgroundMain);
    
    // Draw title bar
    g.setColour(backgroundAccent);
    g.fillRect(0, 0, getWidth(), 60);
    
    // Draw sections
    g.setColour(backgroundAccent);
    g.fillRoundedRectangle(20, 80, 160, 280, 5.0f);  // Oscillator section
    g.fillRoundedRectangle(200, 80, 220, 280, 5.0f); // Filter section
    g.fillRoundedRectangle(440, 80, 240, 280, 5.0f); // Envelope section
    
    // Draw section outlines
    g.setColour(orange);
    g.drawRoundedRectangle(20, 80, 160, 280, 5.0f, 2.0f);
    g.drawRoundedRectangle(200, 80, 220, 280, 5.0f, 2.0f);
    g.drawRoundedRectangle(440, 80, 240, 280, 5.0f, 2.0f);
    
    // Draw title
    g.setColour(orange);
    g.setFont(juce::Font("Arial", 24.0f, juce::Font::bold));
    g.drawFittedText("RETRO SYNTH", 20, 10, getWidth() - 40, 40, juce::Justification::centred, 1);
    
    // Draw section labels
    g.setFont(juce::Font("Arial", 14.0f, juce::Font::bold));
    g.setColour(juce::Colours::white);
    g.drawFittedText("OSCILLATOR", 30, 90, 140, 20, juce::Justification::centred, 1);
    g.drawFittedText("FILTER", 210, 90, 200, 20, juce::Justification::centred, 1);
    g.drawFittedText("ENVELOPE", 450, 90, 220, 20, juce::Justification::centred, 1);
}

void JuceSynthAudioProcessorEditor::resized()
{
    const int knobSize = 70;
    const int labelHeight = 20;
    const int spacing = 10;
    
    // Oscillator section
    waveformKnob->setBounds(60, 130, knobSize, knobSize);
    waveformLabel->setBounds(45, 200, 100, labelHeight);
    
    // Filter section  
    filterCutoffKnob->setBounds(230, 130, knobSize, knobSize);
    filterCutoffLabel->setBounds(215, 200, 100, labelHeight);
    
    filterResonanceKnob->setBounds(320, 130, knobSize, knobSize);
    filterResonanceLabel->setBounds(305, 200, 100, labelHeight);
    
    // Envelope section
    int envX = 460;
    attackKnob->setBounds(envX, 130, knobSize, knobSize);
    attackLabel->setBounds(envX - 15, 200, 100, labelHeight);
    
    envX += 80;
    decayKnob->setBounds(envX, 130, knobSize, knobSize);
    decayLabel->setBounds(envX - 15, 200, 100, labelHeight);
    
    envX += 80;
    sustainKnob->setBounds(envX, 130, knobSize, knobSize);
    sustainLabel->setBounds(envX - 15, 200, 100, labelHeight);
    
    envX += 80;
    releaseKnob->setBounds(envX, 240, knobSize, knobSize);
    releaseLabel->setBounds(envX - 15, 310, 100, labelHeight);
}

void JuceSynthAudioProcessorEditor::setupKnobAndLabel(std::unique_ptr<SynthKnob>& knob, 
                                                     std::unique_ptr<juce::Label>& label,
                                                     const juce::String& labelText)
{
    knob = std::make_unique<SynthKnob>();
    addAndMakeVisible(*knob);
    
    label = std::make_unique<juce::Label>();
    label->setText(labelText, juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);
    label->setFont(juce::Font("Arial", 12.0f, juce::Font::bold));
    label->setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(*label);
}