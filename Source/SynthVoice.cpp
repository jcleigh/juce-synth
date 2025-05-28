#include "SynthVoice.h"
#include <cmath>

SynthVoice::SynthVoice()
    : level(0.0), frequency(0.0), phase(0.0), sampleRate(44100.0), isPlaying(false)
{
    // Set default ADSR parameters
    adsrParams.attack = 0.1f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.5f;
    
    adsr.setParameters(adsrParams);
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

void SynthVoice::prepareToPlay(double sampleRate, int, int)
{
    this->sampleRate = sampleRate;
    adsr.setSampleRate(sampleRate);
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
        const float currentSample = static_cast<float>(std::sin(phase) * level * adsr.getNextSample());
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + sample, currentSample);
        }
        
        phase += phaseIncrement;
        if (phase >= 2.0 * juce::MathConstants<double>::pi)
            phase -= 2.0 * juce::MathConstants<double>::pi;
    }
}