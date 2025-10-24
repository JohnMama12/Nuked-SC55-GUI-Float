#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

class LCD_Backend;

class NukedSC55Controller : public Steinberg::Vst::EditController
{
public:
    NukedSC55Controller();

    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) override;
    Steinberg::tresult PLUGIN_API terminate() override;
    Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state) override;
    Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) override;

    static Steinberg::FUnknown* createInstance(void*)
    {
        return (Steinberg::Vst::IEditController*)new NukedSC55Controller();
    }

    static Steinberg::FUID uid;

    LCD_Backend* m_lcd_backend;
};
