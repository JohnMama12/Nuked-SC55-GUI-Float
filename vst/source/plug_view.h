#pragma once

#include "public.sdk/source/vst/vstguieditor.h"

#if VSTGUI_ENABLED

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ccontrol.h"

namespace VSTGUI {
class CBitmap;
}

class NukedSC55View : public Steinberg::Vst::VSTGUIEditor, public VSTGUI::CControlListener
{
public:
    NukedSC55View(void* controller);

    bool PLUGIN_API open(void* parent, const Steinberg::Vst::PlatformType& platformType) override;
    void PLUGIN_API close() override;

    void valueChanged(VSTGUI::CControl* pControl) override;

private:
    VSTGUI::CBitmap* m_background;
};

#endif
