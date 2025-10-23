#include "controller.h"
#include "editor.h"

namespace VST3 {

Controller::Controller()
{
}

Controller::~Controller()
{
}

Steinberg::tresult PLUGIN_API Controller::initialize(FUnknown* context)
{
    // Call the base class implementation
    Steinberg::tresult result = Steinberg::Vst::EditController::initialize(context);
    if (result != Steinberg::kResultOk) {
        return result;
    }

    return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API Controller::terminate()
{
    // Call the base class implementation
    return Steinberg::Vst::EditController::terminate();
}

Steinberg::Vst::IPlugView* PLUGIN_API Controller::createView(Steinberg::FIDString name)
{
    if (strcmp(name, Steinberg::Vst::ViewType::kEditor) == 0) {
        return new Editor(this);
    }
    return nullptr;
}

} // namespace VST3
