#pragma once

#include "public.sdk/source/vst/vstguieditor.h"

namespace VST3 {

class Editor : public VSTGUI::VST3Editor
{
public:
    Editor(Steinberg::Vst::EditController* controller);
    ~Editor() override;

    // --- VSTGUI::VST3Editor overrides
    bool PLUGIN_API open(void* parent, const VSTGUI::PlatformType& platformType) override;
    void PLUGIN_API close() override;
};

} // namespace VST3
