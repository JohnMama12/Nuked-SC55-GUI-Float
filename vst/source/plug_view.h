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

class NukedSC55View : public Steinberg::Vst::VSTGUIEditor, public VSTGUI::IControlListener
{
public:
    NukedSC55View(void* controller);

    bool PLUGIN_API open(void* parent, const VSTGUI::PlatformType& platformType) override;
    void PLUGIN_API close() override;

    void valueChanged(VSTGUI::CControl* pControl) override;

private:
    VSTGUI::CBitmap* m_background;
    VSTGUI::CView* m_lcdView;
    lcd_t* m_lcd;
};

#endif
