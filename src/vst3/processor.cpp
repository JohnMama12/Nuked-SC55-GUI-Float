#include "processor.h"
#include "backend/audio.h"
#include "win32_helper.h"

namespace VST3 {

Processor::Processor()
{
    // Set the bus arrangements
    addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

    // Add a MIDI input bus
    addEventInput(STR16("MIDI In"));
}

Processor::~Processor()
{
}

Steinberg::tresult PLUGIN_API Processor::initialize(FUnknown* context)
{
    // Call the base class implementation
    Steinberg::tresult result = Steinberg::Vst::AudioEffect::initialize(context);
    if (result != Steinberg::kResultOk) {
        return result;
    }

    // Get the ROM path
    std::filesystem::path romPath = Win32::getRomPathFromRegistry();
    if (romPath.empty()) {
        romPath = Win32::openFolderDialog();
        if (!romPath.empty()) {
            Win32::setRomPathInRegistry(romPath);
        }
    }

    // Create the emulator
    m_emu = std::make_unique<Emu>();
    EMU_Options options;
    options.rom_directory = romPath;
    options.lcd_backend = m_lcdView;
    if (!m_emu->Init(options)) {
        Win32::showErrorMessage("Failed to initialize the emulator.");
        return Steinberg::kResultFalse;
    }

    // Load the ROM set
    if (std::filesystem::exists(romPath / "sc55_rom1.bin")) {
        if (!m_emu->LoadRoms(Romset::MK1)) {
            Win32::showErrorMessage("Failed to load the SC-55 MKI ROM files. Please check that the files are not corrupted and that they are in the correct location.");
            return Steinberg::kResultFalse;
        }
    } else if (std::filesystem::exists(romPath / "rom1.bin")) {
        if (!m_emu->LoadRoms(Romset::MK2)) {
            Win32::showErrorMessage("Failed to load the SC-55 MKII ROM files. Please check that the files are not corrupted and that they are in the correct location.");
            return Steinberg::kResultFalse;
        }
    } else {
        Win32::showErrorMessage("Could not find the SC-55 MKI or MKII ROM files. Please select the folder where the ROM files are located.");
        return Steinberg::kResultFalse;
    }

    // Start the LCD
    m_emu->StartLCD();

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API Processor::terminate()
{
    // Stop the LCD
    if (m_emu) {
        m_emu->StopLCD();
    }

    // Destroy the emulator
    m_emu.reset();

    // Call the base class implementation
    return Steinberg::Vst::AudioEffect::terminate();
}

Steinberg::tresult PLUGIN_API Processor::setActive(Steinberg::TBool state)
{
    if (state) {
        // Set the sample rate
        m_sampleRate = processSetup.sampleRate;
        m_emu->setSampleRate(m_sampleRate);
    }
    return Steinberg::Vst::AudioEffect::setActive(state);
}

Steinberg::tresult PLUGIN_API Processor::process(Steinberg::Vst::ProcessData& data)
{
    // --- Process MIDI events
    Steinberg::Vst::IEventList* eventList = data.inputEvents;
    if (eventList) {
        int32 numEvents = eventList->getEventCount();
        for (int32 i = 0; i < numEvents; i++) {
            Steinberg::Vst::Event event;
            if (eventList->getEvent(i, event) == Steinberg::kResultOk) {
                if (event.type == Steinberg::Vst::Event::kNoteOnEvent) {
                    uint8_t midiData[3];
                    midiData[0] = 0x90 | event.noteOn.channel;
                    midiData[1] = event.noteOn.pitch;
                    midiData[2] = event.noteOn.velocity * 127;
                    m_emu->PostMIDI(midiData);
                } else if (event.type == Steinberg::Vst::Event::kNoteOffEvent) {
                    uint8_t midiData[3];
                    midiData[0] = 0x80 | event.noteOff.channel;
                    midiData[1] = event.noteOff.pitch;
                    midiData[2] = event.noteOff.velocity * 127;
                    m_emu->PostMIDI(midiData);
                } else if (event.type == Steinberg::Vst::Event::kPolyPressureEvent) {
                    uint8_t midiData[3];
                    midiData[0] = 0xA0 | event.polyPressure.channel;
                    midiData[1] = event.polyPressure.pitch;
                    midiData[2] = event.polyPressure.pressure * 127;
                    m_emu->PostMIDI(midiData);
                } else if (event.type == Steinberg::Vst::Event::kDataEvent) {
                    m_emu->PostMIDI({event.data.bytes, (size_t)event.data.size});
                }
            }
        }
    }

    // --- Process the audio
    if (data.numOutputs > 0) {
        AudioSample* out[2];
        out[0] = (AudioSample*)data.outputs[0].channelBuffers32[0];
        out[1] = (AudioSample*)data.outputs[0].channelBuffers32[1];

        // Generate the audio
        m_emu->getSamples(out, data.numSamples);
    }

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API Processor::setState(Steinberg::IBStream* state)
{
    // For now, we don't save any state
    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API Processor::getState(Steinberg::IBStream* state)
{
    // For now, we don't save any state
    return Steinberg::kResultOk;
}

} // namespace VST3
