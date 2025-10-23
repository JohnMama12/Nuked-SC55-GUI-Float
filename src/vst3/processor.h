#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "backend/emu.h"
#include "lcd_view.h"
#include <memory>

namespace VST3 {

class Processor : public Steinberg::Vst::AudioEffect
{
public:
    Processor();
    ~Processor() override;

    // --- Steinberg::Vst::AudioEffect overrides
    Steinberg::tresult PLUGIN_API initialize(FUnknown* context) override;
    Steinberg::tresult PLUGIN_API terminate() override;
    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override;
    Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) override;
    Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) override;

    // --- Static creator
    static Steinberg::FUnknown* createInstance(void*)
    {
        return (Steinberg::Vst::IAudioProcessor*)new Processor();
    }

    void setLCDView(LCDView* lcdView) { m_lcdView = lcdView; }
    Emu* getEmu() const { return m_emu.get(); }

private:
    std::unique_ptr<Emu> m_emu;
    double m_sampleRate = 0.0;
    LCDView* m_lcdView = nullptr;
};

} // namespace VST3
