cmake_minimum_required(VERSION 3.22)

project(JuceSynth VERSION 0.0.1)

# Include the JUCE CMake package
add_subdirectory(JUCE)

# Initialize JUCE with required options
juce_add_plugin(JuceSynth
    VERSION 0.0.1
    COMPANY_NAME "Jordan Cleigh"
    PRODUCT_NAME "JuceSynth"
    BUNDLE_ID "com.jordancleigh.jucesynth"
    FORMATS VST3
    PLUGIN_MANUFACTURER_CODE Jclg
    PLUGIN_CODE Jsyn
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES "Instrument|Synth"
    DESCRIPTION "A simple synthesizer plugin"
)

# Disable webkit to avoid web browser dependencies
target_compile_definitions(JuceSynth PRIVATE 
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
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

# Add GTK include directories for GUI support on Linux
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(JuceSynth PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(JuceSynth PRIVATE ${GTK3_LIBRARIES})
    target_compile_options(JuceSynth PRIVATE ${GTK3_CFLAGS})
endif()

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
        juce::juce_recommended_warning_flags
)

# Add a target for running the plugin
# juce_enable_copy_plugin_step(JuceSynth)  # Commented out to avoid permission errors when building