#include "lcd_view.h"
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cbitmap.h"

namespace VST3 {

LCDView::LCDView(const VSTGUI::CRect& size)
    : VSTGUI::CView(size)
    , m_buffer(std::make_unique<LCD_Pixel[]>(LCD_WIDTH * LCD_HEIGHT))
{
}

LCDView::~LCDView()
{
}

void LCDView::draw(VSTGUI::CDrawContext* pContext)
{
    // Create a bitmap from the buffer
    auto bitmap = VSTGUI::CBitmap(getViewSize());
    VSTGUI::COffscreenContext* offscreenContext = pContext->createOffscreenContext(getViewSize());
    offscreenContext->beginDraw();
    uint8_t* p = (uint8_t*)offscreenContext->getBitmap()->getPlatformBitmap()->getBufferAddress();
    for (int y = 0; y < LCD_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            const LCD_Pixel& pixel = m_buffer[y * LCD_WIDTH + x];
            *p++ = pixel.b;
            *p++ = pixel.g;
            *p++ = pixel.r;
            *p++ = 255;
        }
    }
    offscreenContext->endDraw();
    bitmap.setPlatformBitmap(offscreenContext->getBitmap()->getPlatformBitmap());
    pContext->drawBitmap(&bitmap, getViewSize());
    delete offscreenContext;
}

void LCDView::Refresh(const LCD_Pixel* buffer)
{
    // Copy the buffer
    memcpy(m_buffer.get(), buffer, LCD_WIDTH * LCD_HEIGHT * sizeof(LCD_Pixel));

    // Mark the view as dirty
    setDirty();
}

} // namespace VST3
