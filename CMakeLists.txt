cmake_minimum_required(VERSION 3.15)

project(JuceSynth VERSION 0.0.1)

# Include the JUCE CMake package
add_subdirectory(JUCE)

# Initialize JUCE with required options
juce_add_plugin(JuceSynth
    VERSION 0.0.1
    COMPANY_NAME "Jordan Cleigh"
    PRODUCT_NAME "JuceSynth"
    FORMATS VST3
    PLUGIN_MANUFACTURER_CODE Jclg
    PLUGIN_CODE Jsyn
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    VST3_CATEGORIES Instrument Synth
)

# Add source files
target_sources(JuceSynth
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/SynthVoice.cpp
        Source/SynthVoice.h
        Source/SynthSound.cpp
        Source/SynthSound.h
)

# Include directories
target_include_directories(JuceSynth
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Set C++ standard
target_compile_features(JuceSynth PRIVATE cxx_std_17)

# Add required JUCE modules
target_link_libraries(JuceSynth
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Add a target for running the plugin
juce_enable_copy_plugin_step(JuceSynth)