#pragma once

#include "vstgui/lib/cview.h"
#include "backend/lcd.h"
#include "vstgui/lib/cdrawcontext.h"
#include <memory>

namespace VST3 {

class LCDView : public VSTGUI::CView, public LCD_Backend
{
public:
    LCDView(const VSTGUI::CRect& size);
    ~LCDView() override;

    // --- VSTGUI::CView overrides
    void draw(VSTGUI::CDrawContext* pContext) override;

    // --- LCD_Backend overrides
    void Refresh(const LCD_Pixel* buffer) override;

private:
    std::unique_ptr<LCD_Pixel[]> m_buffer;
};

} // namespace VST3
