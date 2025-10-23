#include "public.sdk/source/main/pluginfactory.h"
#include "processor.h"
#include "controller.h"

//-----------------------------------------------------------------------------
// VST3 VST-Module Entry
//-----------------------------------------------------------------------------
BEGIN_FACTORY_DEF("Nuked", "https://www.nukeykt.com", "mailto:info@nukeykt.com")
    DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::FUID(0x94482070, 0x7A98, 0x4C80, 0xBBFB05368CE302C0)),
               PClassInfo2<VST3::Processor,
                           Steinberg::Vst::IComponent,
                           "Nuked SC-55",
                           "Instrument",
                           "Nuked",
                           Steinberg::Vst::PlugType::kInstrument,
                           "SC-55">,
               &VST3::Processor::createInstance)

    DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::FUID(0x7E00752C, 0x6B54, 0x41B0, 0xA505E73597FBFCEF)),
               PClassInfo2<VST3::Controller,
                           Steinberg::Vst::IEditController,
                           "Nuked SC-55 Controller",
                           "Controller",
                           "Nuked",
                           Steinberg::Vst::PlugType::kInstrument,
                           "SC-55">,
               &VST3::Controller::createInstance)
END_FACTORY
