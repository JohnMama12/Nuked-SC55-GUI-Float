#pragma once

#include "public.sdk/source/vst/vstguieditor.h"
#include "backend/lcd.h"

#if VSTGUI_ENABLED

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ccontrol.h"

namespace VSTGUI {
class CBitmap;
class CView;
}

class NukedSC55View : public Steinberg::Vst::VSTGUIEditor, public VSTGUI::CControlListener, public LCD_Backend
{
public:
    NukedSC55View(void* controller);

    bool PLUGIN_API open(void* parent, const Steinberg::Vst::PlatformType& platformType) override;
    void PLUGIN_API close() override;

    void valueChanged(VSTGUI::CControl* pControl) override;

    // LCD_Backend
    void Render() override;
    bool Start(lcd_t& lcd) override { m_lcd = &lcd; return true; }
    void Stop() override {}

private:
    VSTGUI::CBitmap* m_background;
    VSTGUI::CView* m_lcdView;
    lcd_t* m_lcd;
};

#endif
