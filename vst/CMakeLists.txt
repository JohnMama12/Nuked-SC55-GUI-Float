cmake_minimum_required(VERSION 3.19)
project(NukedSC55VST VERSION 1.0.0)

# VST3 SDK
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/vst3sdk ${CMAKE_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

# VST3 plugin
smtg_add_vst3plugin(NukedSC55VST
    "source/plug.cpp"
    "source/plug_processor.h"
    "source/plug_processor.cpp"
    "source/plug_controller.h"
    "source/plug_controller.cpp"
    "source/plug_view.cpp"
    "source/version.h"
)
target_include_directories(NukedSC55VST PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/vst3sdk
    ${CMAKE_SOURCE_DIR}/vendor/vst3sdk/public.sdk/source/vst
    ${CMAKE_SOURCE_DIR}/vendor/vst3sdk/vstgui4
)
target_compile_features(NukedSC55VST PRIVATE cxx_std_23)
target_link_libraries(NukedSC55VST
    PRIVATE
    sdk
    sdk_hosting
    sdk_common
    PUBLIC
    nuked-sc55-backend
)

# VST3-specific properties
set_target_properties(NukedSC55VST PROPERTIES
    OUTPUT_NAME "Nuked SC-55"
    MACOSX_BUNDLE_NAME "Nuked SC-55"
    XCODE_ATTRIBUTE_PRODUCT_NAME "Nuked SC-55"
)
