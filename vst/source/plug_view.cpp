#include "plug_view.h"
#include "plug_controller.h"
#include "vstgui/lib/cbitmap.h"
#include "vstgui/lib/cframe.h"
#include "vstgui/lib/controls/cbuttons.h"
#include "vstgui/lib/controls/cknob.h"
#include "vstgui/lib/cview.h"

#if VSTGUI_ENABLED

NukedSC55View::NukedSC55View(void* controller)
    : VSTGUIEditor(controller)
    , m_background(nullptr)
    , m_lcdView(nullptr)
    , m_lcd(nullptr)
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

    // LCD view
    VSTGUI::CRect lcdRect(283, 49, 283 + 370, 49 + 134);
    m_lcdView = new VSTGUI::CView(lcdRect);
    frame->addView(m_lcdView);

    // Buttons
    for (int i = 0; i < 32; i++)
    {
        const SDL_Rect* rect = &lcd_button_regions_sc55[i];
        if (rect->x == 0 && rect->y == 0 && rect->w == 0 && rect->h == 0)
            continue;

        VSTGUI::CRect r(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
        VSTGUI::CTextButton* button = new VSTGUI::CTextButton(r, "");
        button->setListener(this);
        button->setTag(i);
        frame->addView(button);
    }

    // Volume knob
    VSTGUI::CRect r(153, 42, 153 + 59, 42 + 59);
    VSTGUI::CKnob* knob = new VSTGUI::CKnob(r, this, 32, m_background, VSTGUI::CPoint(54, 468));
    knob->setMin(0.0f);
    knob->setMax(1.0f);
    knob->setValue(0.8f);
    frame->addView(knob);

    static_cast<NukedSC55Controller*>(controller)->m_lcd_backend = this;

    return frame->open(parent, platformType);
}

void PLUGIN_API NukedSC55View::close()
{
    static_cast<NukedSC55Controller*>(controller)->m_lcd_backend = nullptr;

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
    if (pControl->getTag() >= 0 && pControl->getTag() < 32)
    {
        uint32_t button_pressed = m_lcd->mcu->button_pressed;
        int mask = 1 << pControl->getTag();

        if (pControl->getValue() > 0.5f)
            button_pressed |= mask;
        else
            button_pressed &= ~mask;

        m_lcd->mcu->button_pressed = button_pressed;
    }
    else if (pControl->getTag() == 32)
    {
        m_lcd->volume = pControl->getValue();
    }
}

void NukedSC55View::Render()
{
    if (m_lcdView)
    {
        VSTGUI::CBitmap* lcd_bitmap = new VSTGUI::CBitmap(m_lcd->width, m_lcd->height, VSTGUI::kBGRA);
        VSTGUI::COffscreenContext* context = lcd_bitmap->createBitmapContext();
        context->setDrawMode(VSTGUI::kAliasing);
        context->drawBitmap(new VSTGUI::CBitmap((const VSTGUI::uint8*)m_lcd->buffer, m_lcd->width, m_lcd->height, VSTGUI::kBGRA));
        m_lcdView->setBackground(lcd_bitmap);
        lcd_bitmap->forget();
    }
}

#endif
