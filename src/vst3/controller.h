#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "processor.h"

namespace VST3 {

class Controller : public Steinberg::Vst::EditController
{
public:
    Controller();
    ~Controller() override;

    // --- Steinberg::Vst::EditController overrides
    Steinberg::tresult PLUGIN_API initialize(FUnknown* context) override;
    Steinberg::tresult PLUGIN_API terminate() override;
    Steinberg::Vst::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) override;

    // --- Static creator
    static Steinberg::FUnknown* createInstance(void*)
    {
        return (Steinberg::Vst::IEditController*)new Controller();
    }

    Processor* getProcessor() const { return (Processor*)getComponent(); }
};

} // namespace VST3
