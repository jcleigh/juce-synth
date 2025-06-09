#include "PluginProcessor.h"
#include "PluginEditor.h"

JuceSynthAudioProcessorEditor::JuceSynthAudioProcessorEditor(JuceSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Set the size of the editor window to a 90s synth style
    setSize(800, 400);
    
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
    
    // Setup LFO section
    setupKnobAndLabel(lfoRateKnob, lfoRateLabel, "LFO RATE");
    setupKnobAndLabel(lfoAmountKnob, lfoAmountLabel, "LFO AMOUNT");
    
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
    lfoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "lfoRate", *lfoRateKnob);
    lfoAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(params, "lfoAmount", *lfoAmountKnob);
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
    g.fillRoundedRectangle(20, 80, 140, 280, 5.0f);  // Oscillator section
    g.fillRoundedRectangle(180, 80, 220, 280, 5.0f); // Filter section
    g.fillRoundedRectangle(420, 80, 160, 280, 5.0f); // LFO section
    g.fillRoundedRectangle(600, 80, 180, 280, 5.0f); // Envelope section
    
    // Draw section outlines
    g.setColour(orange);
    g.drawRoundedRectangle(20, 80, 140, 280, 5.0f, 2.0f);
    g.drawRoundedRectangle(180, 80, 220, 280, 5.0f, 2.0f);
    g.drawRoundedRectangle(420, 80, 160, 280, 5.0f, 2.0f);
    g.drawRoundedRectangle(600, 80, 180, 280, 5.0f, 2.0f);
    
    // Draw title
    g.setColour(orange);
    g.setFont(juce::Font("Arial", 24.0f, juce::Font::bold));
    g.drawFittedText("RETRO SYNTH v2.0 - ENHANCED", 20, 10, getWidth() - 40, 40, juce::Justification::centred, 1);
    
    // Draw section labels
    g.setFont(juce::Font("Arial", 14.0f, juce::Font::bold));
    g.setColour(juce::Colours::white);
    g.drawFittedText("OSCILLATOR", 30, 90, 120, 20, juce::Justification::centred, 1);
    g.drawFittedText("FILTER", 190, 90, 200, 20, juce::Justification::centred, 1);
    g.drawFittedText("LFO", 430, 90, 140, 20, juce::Justification::centred, 1);
    g.drawFittedText("ENVELOPE", 610, 90, 160, 20, juce::Justification::centred, 1);
}

void JuceSynthAudioProcessorEditor::resized()
{
    const int knobSize = 70;
    const int labelHeight = 20;
    
    // Oscillator section
    waveformKnob->setBounds(55, 130, knobSize, knobSize);
    waveformLabel->setBounds(40, 200, 100, labelHeight);
    
    // Filter section  
    filterCutoffKnob->setBounds(210, 130, knobSize, knobSize);
    filterCutoffLabel->setBounds(195, 200, 100, labelHeight);
    
    filterResonanceKnob->setBounds(300, 130, knobSize, knobSize);
    filterResonanceLabel->setBounds(285, 200, 100, labelHeight);
    
    // LFO section
    lfoRateKnob->setBounds(440, 130, knobSize, knobSize);
    lfoRateLabel->setBounds(425, 200, 100, labelHeight);
    
    lfoAmountKnob->setBounds(520, 130, knobSize, knobSize);
    lfoAmountLabel->setBounds(505, 200, 100, labelHeight);
    
    // Envelope section
    int envX = 620;
    attackKnob->setBounds(envX, 130, knobSize, knobSize);
    attackLabel->setBounds(envX - 15, 200, 100, labelHeight);
    
    envX += 80;
    decayKnob->setBounds(envX, 130, knobSize, knobSize);
    decayLabel->setBounds(envX - 15, 200, 100, labelHeight);
    
    sustainKnob->setBounds(620, 240, knobSize, knobSize);
    sustainLabel->setBounds(605, 310, 100, labelHeight);
    
    releaseKnob->setBounds(700, 240, knobSize, knobSize);
    releaseLabel->setBounds(685, 310, 100, labelHeight);
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