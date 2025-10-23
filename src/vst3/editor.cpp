#include "editor.h"
#include "vstgui/lib/cbitmap.h"
#include "vstgui/lib/controls/ctextlabel.h"

namespace VST3 {

Editor::Editor(Steinberg::Vst::EditController* controller)
    : VSTGUI::VST3Editor(controller)
{
    // Set the size of the editor
    rect.setSize(400, 200);
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

    // Add a label
    auto* label = new VSTGUI::CTextLabel(VSTGUI::CRect(10, 10, 390, 30), "Nuked SC-55");
    label->setTextColor(VSTGUI::kWhiteCColor);
    frame->addView(label);

    return true;
}

void PLUGIN_API Editor::close()
{
    if (frame) {
        frame->close();
        frame = nullptr;
    }
}

} // namespace VST3
