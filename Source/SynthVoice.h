#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    enum WaveformType
    {
        Sine = 0,
        Saw,
        Square,
        Triangle,
        Noise
    };
    
    SynthVoice();
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    // Parameter setters for external control
    void setWaveform(WaveformType waveform) { currentWaveform = waveform; }
    void setFilterCutoff(float cutoff) { filterCutoff = cutoff; baseCutoff = cutoff; updateFilter(); }
    void setFilterResonance(float resonance) { filterResonance = resonance; updateFilter(); }
    void setADSRParameters(const juce::ADSR::Parameters& params) { adsrParams = params; adsr.setParameters(adsrParams); }
    void setLFORate(float rate) { lfoRate = rate; }
    void setLFOAmount(float amount) { lfoAmount = amount; }
    
private:
    double level;
    double frequency;
    double phase;
    double sampleRate;
    
    bool isPlaying;
    
    // Oscillator
    WaveformType currentWaveform;
    
    // Filter
    float filterCutoff;
    float filterResonance;
    float baseCutoff; // Store the base cutoff for LFO modulation
    juce::dsp::StateVariableTPTFilter<float> filter;
    
    // LFO
    float lfoRate;
    float lfoAmount;
    double lfoPhase;
    
    // ADSR envelope
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    // Random number generator for noise
    juce::Random random;
    
    // Helper methods
    float generateWaveform();
    void updateFilter();
};