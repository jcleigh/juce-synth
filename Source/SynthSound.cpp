#include "SynthSound.h"

SynthSound::SynthSound()
{
}

bool SynthSound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool SynthSound::appliesToChannel(int midiChannel)
{
    return true;
}