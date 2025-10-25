#pragma once

#include "../../vendor/vst3sdk/public.sdk/source/vst/vstaudioeffect.h"
#include "backend/emu.h"

class NukedSC55Processor : public Steinberg::Vst::AudioEffect
{
public:
    NukedSC55Processor();

    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) override;
    Steinberg::tresult PLUGIN_API terminate() override;
    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override;
    Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) override;
    Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) override;
    Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) override;

    static Steinberg::FUnknown* createInstance(void*)
    {
        return (Steinberg::Vst::IAudioProcessor*)new NukedSC55Processor();
    }

    static Steinberg::FUID uid;

private:
    std::unique_ptr<Emulator> m_emulator;
};
