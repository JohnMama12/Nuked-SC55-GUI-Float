cmake_minimum_required(VERSION 3.19)
project(NukedSC55VST)

# VST3 SDK
add_subdirectory(../vendor/vst3sdk ${CMAKE_CURRENT_BINARY_DIR}/vst3sdk)

# Backend library
add_library(nuked-sc55-backend
    ../src/backend/config.cpp
    ../src/backend/emu.cpp
    ../src/backend/lcd.cpp
    ../src/backend/mcu.cpp
    ../src/backend/mcu_interrupt.cpp
    ../src/backend/mcu_opcodes.cpp
    ../src/backend/mcu_timer.cpp
    ../src/backend/path_util.cpp
    ../src/backend/pcm.cpp
    ../src/backend/submcu.cpp
)
target_include_directories(nuked-sc55-backend
    PUBLIC
    ../src/backend
    ${CMAKE_CURRENT_BINARY_DIR}/backend
)
target_compile_features(nuked-sc55-backend PRIVATE cxx_std_23)

# VST3 plugin
add_library(NukedSC55VST MODULE
    "source/plug.cpp"
    "source/plug_processor.cpp"
    "source/plug_controller.cpp"
    "source/version.h"
)
target_link_libraries(NukedSC55VST
    PRIVATE
    vstsdk
    PUBLIC
    nuked-sc55-backend
)

# VST3-specific properties
set_target_properties(NukedSC55VST PROPERTIES
    OUTPUT_NAME "Nuked SC-55"
    MACOSX_BUNDLE_NAME "Nuked SC-55"
    XCODE_ATTRIBUTE_PRODUCT_NAME "Nuked SC-55"
)
