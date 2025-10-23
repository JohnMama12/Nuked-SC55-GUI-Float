#pragma once

#include "vstgui/vst3/vst3editor.h"
#include "lcd_view.h"

namespace VST3 {

class Editor : public VSTGUI::VST3Editor
{
public:
    Editor(Steinberg::Vst::EditController* controller);
    ~Editor() override;

    // --- VSTGUI::VST3Editor overrides
    bool PLUGIN_API open(void* parent, const VSTGUI::PlatformType& platformType) override;
    int32_t PLUGIN_API onKeyDown(VSTGUI::VstKeyCode& keyCode) override;
    int32_t PLUGIN_API onKeyUp(VSTGUI::VstKeyCode& keyCode) override;

private:
    LCDView* m_lcdView = nullptr;
};

} // namespace VST3
