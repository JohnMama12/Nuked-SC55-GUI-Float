#include "plug_view.h"
#include "plug_controller.h"
#include "vstgui/lib/cbitmap.h"
#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/cbuttons.h"
#include "vstgui/lib/controls/cknob.h"

#if VSTGUI_ENABLED

NukedSC55View::NukedSC55View(void* controller)
    : VSTGUIEditor(controller, "NukedSC55View", "nuked_sc55.uidesc")
    , m_background(nullptr)
{
}

bool PLUGIN_API NukedSC55View::open(void* parent, const Steinberg::Vst::PlatformType& platformType)
{
    if (frame)
    {
        close();
    }

    m_background = new VSTGUI::CBitmap("sc55_background.bmp");
    if (!m_background)
    {
        return false;
    }

    VSTGUI::CRect size(0, 0, m_background->getWidth(), m_background->getHeight());
    frame = new VSTGUI::CFrame(size, this);
    frame->setBackground(m_background);

    // TODO: Create controls and replicate the rendering logic from lcd_sdl.cpp

    return frame->open(parent, platformType);
}

void PLUGIN_API NukedSC55View::close()
{
    if (frame)
    {
        frame->close();
        frame = nullptr;
    }

    if (m_background)
    {
        m_background->forget();
        m_background = nullptr;
    }
}

void NukedSC55View::valueChanged(VSTGUI::CControl* pControl)
{
    // TODO: Handle control value changes
}

#endif
