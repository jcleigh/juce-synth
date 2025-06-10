#include "PluginProcessor.h"
#include "PluginEditor.h"

JuceSynthAudioProcessor::JuceSynthAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS",
      {
          std::make_unique<juce::AudioParameterChoice>("waveform", "Waveform", 
              juce::StringArray{"Sine", "Saw", "Square", "Triangle", "Noise"}, 1),
          std::make_unique<juce::AudioParameterFloat>("filterCutoff", "Filter Cutoff",
              juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 8000.0f),
          std::make_unique<juce::AudioParameterFloat>("filterResonance", "Filter Resonance",
              juce::NormalisableRange<float>(0.1f, 2.0f, 0.01f), 0.7f),
          std::make_unique<juce::AudioParameterFloat>("attack", "Attack",
              juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.1f),
          std::make_unique<juce::AudioParameterFloat>("decay", "Decay",
              juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.3f),
          std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.6f),
          std::make_unique<juce::AudioParameterFloat>("release", "Release",
              juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.8f),
          std::make_unique<juce::AudioParameterFloat>("lfoRate", "LFO Rate",
              juce::NormalisableRange<float>(0.1f, 20.0f, 0.1f), 2.0f),
          std::make_unique<juce::AudioParameterFloat>("lfoAmount", "LFO Amount",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f)
      })
{
    // Get parameter pointers
    waveformParam = parameters.getRawParameterValue("waveform");
    filterCutoffParam = parameters.getRawParameterValue("filterCutoff");
    filterResonanceParam = parameters.getRawParameterValue("filterResonance");
    attackParam = parameters.getRawParameterValue("attack");
    decayParam = parameters.getRawParameterValue("decay");
    sustainParam = parameters.getRawParameterValue("sustain");
    releaseParam = parameters.getRawParameterValue("release");
    lfoRateParam = parameters.getRawParameterValue("lfoRate");
    lfoAmountParam = parameters.getRawParameterValue("lfoAmount");
    
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

bool JuceSynthAudioProcessor::supportsDoublePrecisionProcessing() const
{
    return false;
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
    // This is a synthesizer: no audio input, stereo output only
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::disabled())
        return false;
        
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

    // Update voice parameters
    updateVoiceParameters();

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
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void JuceSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void JuceSynthAudioProcessor::updateVoiceParameters()
{
    const auto waveform = static_cast<SynthVoice::WaveformType>(static_cast<int>(waveformParam->load()));
    const auto cutoff = filterCutoffParam->load();
    const auto resonance = filterResonanceParam->load();
    const auto lfoRate = lfoRateParam->load();
    const auto lfoAmount = lfoAmountParam->load();
    
    juce::ADSR::Parameters adsrParams;
    adsrParams.attack = attackParam->load();
    adsrParams.decay = decayParam->load();
    adsrParams.sustain = sustainParam->load();
    adsrParams.release = releaseParam->load();
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setWaveform(waveform);
            voice->setFilterCutoff(cutoff);
            voice->setFilterResonance(resonance);
            voice->setADSRParameters(adsrParams);
            voice->setLFORate(lfoRate);
            voice->setLFOAmount(lfoAmount);
        }
    }
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceSynthAudioProcessor();
}