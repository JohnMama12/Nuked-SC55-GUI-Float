#include "plug_processor.h"
#include "plug_controller.h"
#include "backend/emu.h"
#include <public.sdk/source/vst/vstaudioprocessoralgo.h>

Steinberg::FUID NukedSC55Processor::uid(0x12345678, 0x90ABCDEF, 0x12345678, 0x90ABCDEF);

NukedSC55Processor::NukedSC55Processor()
    : m_emulator(std::make_unique<Emulator>())
{
    setControllerClass(NukedSC55Controller::uid);
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::initialize(Steinberg::FUnknown* context)
{
    tresult result = AudioEffect::initialize(context);
    if (result != Steinberg::kResultOk)
    {
        return result;
    }

    addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);
    addEventInput(STR16("Event In"), 1);

    if (getController())
    {
        getController()->setDirty(true);
    }

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::terminate()
{
    return AudioEffect::terminate();
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::setActive(Steinberg::TBool state)
{
    return AudioEffect::setActive(state);
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::process(Steinberg::Vst::ProcessData& data)
{
    if (data.numSamples > 0)
    {
        // Process MIDI events
        Steinberg::Vst::IEventList* eventList = data.inputEvents;
        if (eventList)
        {
            int32 numEvents = eventList->getEventCount();
            for (int32 i = 0; i < numEvents; i++)
            {
                Steinberg::Vst::Event event;
                if (eventList->getEvent(i, event) == Steinberg::kResultOk)
                {
                    switch (event.type)
                    {
                    case Steinberg::Vst::Event::kNoteOnEvent:
                        m_emulator->PostMidi(0x90 | (event.noteOn.channel & 0x0F));
                        m_emulator->PostMidi(event.noteOn.pitch);
                        m_emulator->PostMidi(event.noteOn.velocity * 127.0f);
                        break;
                    case Steinberg::Vst::Event::kNoteOffEvent:
                        m_emulator->PostMidi(0x80 | (event.noteOff.channel & 0x0F));
                        m_emulator->PostMidi(event.noteOff.pitch);
                        m_emulator->PostMidi(event.noteOff.velocity * 127.0f);
                        break;
                    case Steinberg::Vst::Event::kPolyPressureEvent:
                        m_emulator->PostMidi(0xA0 | (event.polyPressure.channel & 0x0F));
                        m_emulator->PostMidi(event.polyPressure.pitch);
                        m_emulator->PostMidi(event.polyPressure.pressure * 127.0f);
                        break;
                    case Steinberg::Vst::Event::kDataEvent:
                        m_emulator->PostMidi(event.data.bytes, event.data.size);
                        break;
                    }
                }
            }
        }

        // Generate audio
        Steinberg::Vst::AudioBusBuffers& outputBuffers = data.outputs[0];
        float* leftBuffer = outputBuffers.channelBuffers32[0];
        float* rightBuffer = outputBuffers.channelBuffers32[1];

        struct AudioBuffer
        {
            float* left;
            float* right;
        } buffer = {leftBuffer, rightBuffer};

        m_emulator->SetSampleCallback(
            [](void* user, int16_t left, int16_t right) {
                AudioBuffer* buffer = static_cast<AudioBuffer*>(user);
                *buffer->left++ = left / 32768.0f;
                *buffer->right++ = right / 32768.0f;
            },
            &buffer);

        for (int32 i = 0; i < data.numSamples; i++)
        {
            m_emulator->Step();
        }
    }

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::canProcessSampleSize(Steinberg::int32 symbolicSampleSize)
{
    if (symbolicSampleSize == Steinberg::Vst::kSample32)
    {
        return Steinberg::kResultTrue;
    }

    return Steinberg::kResultFalse;
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::setState(Steinberg::IBStream* state)
{
    if (!state)
    {
        return Steinberg::kInvalidArgument;
    }

    m_emulator->SaveState([&](const void* data, size_t size) {
        state->write(data, size);
    });

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API NukedSC55Processor::getState(Steinberg::IBStream* state)
{
    if (!state)
    {
        return Steinberg::kInvalidArgument;
    }

    m_emulator->LoadState([&](void* data, size_t size) {
        state->read(data, size);
    });

    return Steinberg::kResultOk;
}
