#include "plug_controller.h"
#include "plug_view.h"

Steinberg::FUID NukedSC55Controller::uid(0x90ABCDEF, 0x12345678, 0x90ABCDEF, 0x12345678);

NukedSC55Controller::NukedSC55Controller()
{
}

Steinberg::tresult PLUGIN_API NukedSC55Controller::initialize(Steinberg::FUnknown* context)
{
    tresult result = EditController::initialize(context);
    return result;
}

Steinberg::tresult PLUGIN_API NukedSC55Controller::terminate()
{
    return EditController::terminate();
}

Steinberg::tresult PLUGIN_API NukedSC55Controller::setComponentState(Steinberg::IBStream* state)
{
    // TODO: Implement loading the controller state
    return Steinberg::kResultOk;
}

Steinberg::IPlugView* PLUGIN_API NukedSC55Controller::createView(Steinberg::FIDString name)
{
    if (strcmp(name, Steinberg::Vst::ViewType::kEditor) == 0)
    {
        return new NukedSC55View(this);
    }
    return nullptr;
}
