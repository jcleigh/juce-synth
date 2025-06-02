#include "SynthVoice.h"
#include <cmath>

SynthVoice::SynthVoice()
    : level(0.0), frequency(0.0), phase(0.0), sampleRate(44100.0), isPlaying(false),
      currentWaveform(Sine), filterCutoff(1000.0f), filterResonance(0.7f)
{
    // Set default ADSR parameters
    adsrParams.attack = 0.1f;
    adsrParams.decay = 0.3f;
    adsrParams.sustain = 0.6f;
    adsrParams.release = 0.8f;
    
    adsr.setParameters(adsrParams);
    
    // Initialize filter
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int)
{
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    level = velocity * 0.15;
    phase = 0.0;
    isPlaying = true;
    
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
        isPlaying = false;
    }
}

void SynthVoice::controllerMoved(int, int)
{
    // Not implemented for this simple synth
}

void SynthVoice::pitchWheelMoved(int)
{
    // Not implemented for this simple synth
}

void SynthVoice::prepareToPlay(double sr, int samplesPerBlock, int)
{
    sampleRate = sr;
    adsr.setSampleRate(sr);
    
    // Prepare filter
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 1;
    
    filter.prepare(spec);
    updateFilter();
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isPlaying)
        return;
    
    if (!adsr.isActive())
    {
        clearCurrentNote();
        isPlaying = false;
        return;
    }
    
    const double phaseIncrement = 2.0 * juce::MathConstants<double>::pi * frequency / sampleRate;
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float waveformSample = generateWaveform();
        const float envelopedSample = waveformSample * level * adsr.getNextSample();
        
        // Apply filter
        const float filteredSample = filter.processSample(0, envelopedSample);
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + sample, filteredSample);
        }
        
        phase += phaseIncrement;
        if (phase >= 2.0 * juce::MathConstants<double>::pi)
            phase -= 2.0 * juce::MathConstants<double>::pi;
    }
}

float SynthVoice::generateWaveform()
{
    switch (currentWaveform)
    {
        case Sine:
            return static_cast<float>(std::sin(phase));
            
        case Saw:
            return static_cast<float>(2.0 * (phase / (2.0 * juce::MathConstants<double>::pi)) - 1.0);
            
        case Square:
            return (phase < juce::MathConstants<double>::pi) ? 1.0f : -1.0f;
            
        case Triangle:
        {
            const double normalizedPhase = phase / (2.0 * juce::MathConstants<double>::pi);
            if (normalizedPhase < 0.5)
                return static_cast<float>(4.0 * normalizedPhase - 1.0);
            else
                return static_cast<float>(3.0 - 4.0 * normalizedPhase);
        }
            
        case Noise:
            return random.nextFloat() * 2.0f - 1.0f;
            
        default:
            return static_cast<float>(std::sin(phase));
    }
}

void SynthVoice::updateFilter()
{
    filter.setCutoffFrequency(filterCutoff);
    filter.setResonance(filterResonance);
}