# juce-synth
Simple starter synth using JUCE framework

This is a basic "Hello World" synthesizer VST3 plugin built with the JUCE framework. It generates a simple sine wave with ADSR envelope control in response to MIDI input.

## Features

- Sine wave oscillator
- ADSR envelope
- 8-voice polyphony
- MIDI note input
- VST3 plugin format

## Building the Project

### Prerequisites

- CMake (version 3.22 or higher)
- C++ compiler supporting C++17
- Git

### Build Steps

1. Clone this repository with submodules:
   ```
   git clone --recursive https://github.com/jcleigh/juce-synth.git
   ```

2. Create a build directory:
   ```
   cd juce-synth
   mkdir build
   cd build
   ```

3. Configure and build:
   ```
   cmake ..
   cmake --build .
   ```

4. The VST3 plugin will be created in the `build/JuceSynth_artefacts/VST3/` directory.

5. **Install the plugin manually** (optional):
   - **Windows**: Copy `JuceSynth.vst3` to `C:\Program Files\Common Files\VST3\`
   - **macOS**: Copy `JuceSynth.vst3` to `~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`
   - **Linux**: Copy `JuceSynth.vst3` to `~/.vst3/` or `/usr/local/lib/vst3/`

## Usage

1. Install the VST3 plugin to your DAW's VST plugin directory (see step 5 above)
2. Open your DAW and scan for new plugins
3. Create a new track with the JuceSynth plugin
4. Play MIDI notes to hear the synthesizer

## License

This project is licensed under the MIT License - see the LICENSE file for details.
