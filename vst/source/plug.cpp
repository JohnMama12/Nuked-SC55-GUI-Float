#include "plug_processor.h"
#include "plug_controller.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define kPluginName "Nuked SC-55"

BEGIN_FACTORY_DEF(kPluginName, "https://github.com/jcmoyer/Nuked-SC55", "mailto:jcmoyer@tuta.io")

DEF_CLASS2(INLINE_UID_FROM_FUID(NukedSC55Processor::uid),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    kPluginName,
    Vst::kDistributable,
    "Instrument",
    FULL_VERSION_STR,
    kVstVersionString,
    NukedSC55Processor::createInstance)

DEF_CLASS2(INLINE_UID_FROM_FUID(NukedSC55Controller::uid),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    kPluginName,
    Vst::kDistributable,
    "Instrument",
    FULL_VERSION_STR,
    kVstVersionString,
    NukedSC55Controller::createInstance)

END_FACTORY

bool InitModule() { return true; }
bool DeinitModule() { return true; }
