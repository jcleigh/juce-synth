#include "PluginProcessor.h"
#include "PluginEditor.h"

JuceSynthAudioProcessor::JuceSynthAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    // Initialize the synthesizer with voices
    for (int i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new SynthVoice());
    }
    
    // Add our sound
    synth.addSound(new SynthSound());
}

JuceSynthAudioProcessor::~JuceSynthAudioProcessor()
{
}

const juce::String JuceSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceSynthAudioProcessor::acceptsMidi() const
{
    return true;
}

bool JuceSynthAudioProcessor::producesMidi() const
{
    return false;
}

bool JuceSynthAudioProcessor::isMidiEffect() const
{
    return false;
}

double JuceSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceSynthAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String JuceSynthAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void JuceSynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void JuceSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void JuceSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool JuceSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // We only support stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void JuceSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process the synthesizer with MIDI messages and generate audio
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

bool JuceSynthAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JuceSynthAudioProcessor::createEditor()
{
    return new JuceSynthAudioProcessorEditor(*this);
}

void JuceSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void JuceSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceSynthAudioProcessor();
}