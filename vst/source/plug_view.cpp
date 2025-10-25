#include "plug_view.h"

#if VSTGUI_ENABLED

#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/ccontrol.h"

class NukedSC55Controller;

NukedSC55View::NukedSC55View(void* controller)
    : VSTGUIEditor(controller)
{
}

bool PLUGIN_API NukedSC55View::open(void* parent, const VSTGUI::PlatformType& platformType)
{
    // Create the frame
    CRect frameSize(0, 0, 500, 200);
    frame = new CFrame(frameSize, this);
    frame->open(parent);

    // Create the background
    m_background = new CBitmap("sc55_background.bmp");
    frame->setBackground(m_background);

    return true;
}

void PLUGIN_API NukedSC55View::close()
{
    if (frame)
    {
        frame->close();
        delete frame;
        frame = nullptr;
    }
    if (m_background)
    {
        delete m_background;
        m_background = nullptr;
    }
}

void NukedSC55View::valueChanged(CControl* pControl)
{
}

#endif
