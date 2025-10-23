#include "editor.h"
#include "lcd_view.h"
#include "controller.h"
#include "backend/mcu.h"

namespace VST3 {

Editor::Editor(Steinberg::Vst::EditController* controller)
    : VSTGUI::VST3Editor(controller)
{
    // Set the size of the editor
    rect.setSize(LCD_WIDTH, LCD_HEIGHT);
}

Editor::~Editor()
{
}

bool PLUGIN_API Editor::open(void* parent, const VSTGUI::PlatformType& platformType)
{
    // Create the frame
    if (frame == nullptr) {
        frame = new VSTGUI::CFrame(rect, this);
        frame->open(parent);
    }

    // Set the background color
    frame->setBackgroundColor(VSTGUI::kBlackCColor);

    // Add the LCD view
    m_lcdView = new LCDView(rect);
    frame->addView(m_lcdView);

    // Pass the LCD view to the processor
    if (auto* controller = (Controller*)getController()) {
        if (auto* processor = controller->getProcessor()) {
            processor->setLCDView(m_lcdView);
        }
    }

    return true;
}

int32_t PLUGIN_API Editor::onKeyDown(VSTGUI::VstKeyCode& keyCode)
{
    int mask = 0;
    switch (keyCode.character) {
        case 'q': mask = 1 << MCU_BUTTON_POWER; break;
        case 'w': mask = 1 << MCU_BUTTON_INST_ALL; break;
        case 'e': mask = 1 << MCU_BUTTON_INST_MUTE; break;
        case 'r': mask = 1 << MCU_BUTTON_PART_L; break;
        case 't': mask = 1 << MCU_BUTTON_PART_R; break;
        case 'y': mask = 1 << MCU_BUTTON_INST_L; break;
        case 'u': mask = 1 << MCU_BUTTON_INST_R; break;
        case 'i': mask = 1 << MCU_BUTTON_KEY_SHIFT_L; break;
        case 'o': mask = 1 << MCU_BUTTON_KEY_SHIFT_R; break;
        case 'p': mask = 1 << MCU_BUTTON_LEVEL_L; break;
        case '[': mask = 1 << MCU_BUTTON_LEVEL_R; break;
        case 'a': mask = 1 << MCU_BUTTON_MIDI_CH_L; break;
        case 's': mask = 1 << MCU_BUTTON_MIDI_CH_R; break;
        case 'd': mask = 1 << MCU_BUTTON_PAN_L; break;
        case 'f': mask = 1 << MCU_BUTTON_PAN_R; break;
        case 'g': mask = 1 << MCU_BUTTON_REVERB_L; break;
        case 'h': mask = 1 << MCU_BUTTON_REVERB_R; break;
        case 'j': mask = 1 << MCU_BUTTON_CHORUS_L; break;
        case 'k': mask = 1 << MCU_BUTTON_CHORUS_R; break;
    }

    if (mask != 0) {
        if (auto* controller = (Controller*)getController()) {
            if (auto* processor = controller->getProcessor()) {
                processor->getEmu()->GetMCU().button_pressed |= mask;
            }
        }
        return 1;
    }
    return -1;
}

int32_t PLUGIN_API Editor::onKeyUp(VSTGUI::VstKeyCode& keyCode)
{
    int mask = 0;
    switch (keyCode.character) {
        case 'q': mask = 1 << MCU_BUTTON_POWER; break;
        case 'w': mask = 1 << MCU_BUTTON_INST_ALL; break;
        case 'e': mask = 1 << MCU_BUTTON_INST_MUTE; break;
        case 'r': mask = 1 << MCU_BUTTON_PART_L; break;
        case 't': mask = 1 << MCU_BUTTON_PART_R; break;
        case 'y': mask = 1 << MCU_BUTTON_INST_L; break;
        case 'u': mask = 1 << MCU_BUTTON_INST_R; break;
        case 'i': mask = 1 << MCU_BUTTON_KEY_SHIFT_L; break;
        case 'o': mask = 1 << MCU_BUTTON_KEY_SHIFT_R; break;
        case 'p': mask = 1 << MCU_BUTTON_LEVEL_L; break;
        case '[': mask = 1 << MCU_BUTTON_LEVEL_R; break;
        case 'a': mask = 1 << MCU_BUTTON_MIDI_CH_L; break;
        case 's': mask = 1 << MCU_BUTTON_MIDI_CH_R; break;
        case 'd': mask = 1 << MCU_BUTTON_PAN_L; break;
        case 'f': mask = 1 << MCU_BUTTON_PAN_R; break;
        case 'g': mask = 1 << MCU_BUTTON_REVERB_L; break;
        case 'h': mask = 1 << MCU_BUTTON_REVERB_R; break;
        case 'j': mask = 1 << MCU_BUTTON_CHORUS_L; break;
        case 'k': mask = 1 << MCU_BUTTON_CHORUS_R; break;
    }

    if (mask != 0) {
        if (auto* controller = (Controller*)getController()) {
            if (auto* processor = controller->getProcessor()) {
                processor->getEmu()->GetMCU().button_pressed &= ~mask;
            }
        }
        return 1;
    }
    return -1;
}

} // namespace VST3
