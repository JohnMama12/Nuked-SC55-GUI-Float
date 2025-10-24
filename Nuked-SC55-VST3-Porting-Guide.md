# Nuked-SC55 VST3 Porting Guide

This document provides a comprehensive technical overview of the Nuked-SC55 emulator and a detailed plan for porting it to a VST3 plugin. The target audience for this document is a software engineer tasked with the porting effort.

## 1. Project Overview

The project is a C++ emulator for several Roland Sound Canvas and related MIDI synthesizer modules. It is currently a standalone application that uses SDL2 for its GUI and audio output. The goal is to port this application to a VST3 plugin, allowing it to be used within a Digital Audio Workstation (DAW).

### 1.1. Supported Emulation Targets

*   SC-55mk2/SC-155mk2 (v1.01 firmware)
*   SC-55mk1 (v1.0/v1.21/v2.0 firmwares)
*   CM-300/SCC-1 (v1.10/v1.20 firmwares)
*   SCC-1A
*   SC-55st (v1.01)
*   JV-880 (v1.0.0/v1.0.1)
*   SCB-55/RLP-3194
*   RLP-3237
*   SC-155

### 1.2. Directory Structure

The repository is organized into the following main directories:

*   `src/`: Contains all the source code for the emulator.
    *   `backend/`: The core emulation library. This is a self-contained, platform-agnostic library that implements the CPU, sound synthesis, and other hardware components.
    *   `standard/`: The standalone application that uses the `backend` library. It provides a reference implementation for how to use the library.
*   `roms/`: Contains the firmware ROM files for the various emulated targets.
*   `vst/`: (To be created) Will contain the VST3 plugin source code.
*   `vendor/`: Contains third-party libraries, including the Steinberg VST3 SDK.

## 2. Codebase Analysis (Deep Dive)

This section provides a detailed breakdown of the key source files in the project.

### 2.1. `src/backend` - The Core Emulator Library

The `backend` directory contains the heart of the emulator. It is designed to be highly portable and has minimal dependencies.

#### 2.1.1. `emu.cpp` / `.h` - The Emulator Core

*   **Purpose:** This is the central orchestrator of the entire emulator. The `Emulator` class encapsulates all hardware components and manages the main emulation loop.
*   **Key Components:**
    *   `Emulator` class: The primary interface for the emulator. It holds instances of all emulated hardware (`mcu`, `pcm`, `lcd`, `submcu`, etc.).
    *   `m_unscramble_rom` function: A proprietary function to unscramble the waveform ROM data. This is a critical step during initialization.
    *   `Step()` method: The main emulation loop. It calls `MCU_Step()` to advance the CPU by a single instruction and updates all peripherals accordingly.
    *   Callbacks:
        *   `SetSampleCallback(SampleCallback cb, void* user)`: Registers a callback function to receive generated audio samples. This is the primary mechanism for getting audio out of the emulator.
        *   `SetMidiOutCallback(MidiOutCallback cb, void* user)`: Registers a callback to handle MIDI output from the emulated device.
    *   `LCD_Backend` abstract class: Defines the interface for rendering the LCD display, decoupling the core emulation from the specific GUI implementation.

#### 2.1.2. `mcu.cpp` / `.h` - Main CPU Emulation

*   **Purpose:** Emulates the main NEC µPD78C14 CPU.
*   **Key Components:**
    *   `mcu_t` struct: A large struct that holds the entire state of the CPU, including registers (`a`, `v`, `b`, `c`, `d`, `e`, `h`, `l`, `pc`, `sp`, etc.), memory maps (`rom`, `ram`, `sram`), and I/O registers.
    *   `MCU_Reset()`: Initializes the CPU to its power-on state.
    *   `MCU_Step()`: Fetches the next opcode from memory and executes it using a function pointer table (`MCU_Operand_Table`).
    *   `MCU_Read(uint16_t addr)` / `MCU_Write(uint16_t addr, uint8_t val)`: These functions implement the CPU's memory map. They handle reading from and writing to different memory regions (ROM, RAM, SRAM) and also map specific addresses to peripheral control registers (e.g., PCM, LCD, timers). This is where the CPU communicates with the other hardware components.

#### 2.1.3. `mcu_opcodes.cpp` / `.h`

*   **Purpose:** Contains the implementation for each individual CPU instruction.
*   **Key Components:**
    *   `MCU_Operand_Table`: A large array of function pointers, indexed by opcode. This acts as a dispatch table for efficient instruction execution.
    *   Instruction implementation functions (e.g., `op_LD_A_V`, `op_ADD_A_B`, `op_JR_c`, etc.): Each function implements the logic for a single opcode, modifying the CPU state (`mcu_t`) as required.
    *   `MCU_Operand_General()`: A complex function that decodes the various addressing modes of the CPU instructions.

#### 2.1.4. `mcu_interrupt.cpp` / `.h`

*   **Purpose:** Emulates the CPU's interrupt controller.
*   **Key Components:**
    *   `MCU_RequestInterrupt(int level)`: Called by peripherals to signal an interrupt request.
    *   `MCU_AcknowledgeInterrupt()`: Checks for pending interrupts at the beginning of each instruction cycle. If an interrupt is pending and its priority is high enough, it performs a context switch (saving the current program counter and flags to the stack) and jumps to the appropriate interrupt service routine.

#### 2.1.5. `pcm.cpp` / `.h` - Sound Synthesis

*   **Purpose:** Emulates the Roland PCM sound chip, which is responsible for all sound generation.
*   **Key Components:**
    *   `PCM_Update(int16_t* buffer, int samples)`: The main function that generates a block of audio samples. It iterates through all 32 channels of the sound chip.
    *   For each channel, it performs:
        1.  Waveform address generation and looping.
        2.  DPCM sample decompression.
        3.  Pitch shifting via interpolation.
        4.  Applying a digital filter (TVF).
        5.  Applying a volume envelope (TVA).
    *   The outputs of all channels are then mixed together.
    *   Global effects (reverb and chorus) are applied using `eram` as a delay line.
    *   The final mixed samples are passed to the host via the `MCU_PostSample` callback.

#### 2.1.6. `lcd.cpp` / `.h` - LCD Controller

*   **Purpose:** Emulates the character-based LCD controller.
*   **Key Components:**
    *   `LCD_Write(int dc, uint8_t data)`: Processes commands and data sent from the main CPU. It updates an internal character memory (`LCD_Data`).
    *   `LCD_Render()`: Renders the characters from `LCD_Data` into a raw pixel buffer (`lcd_t::buffer`) using a bitmap font map (`lcd_font`).
    *   The `LCD_Backend` interface is used to transfer this pixel buffer to the frontend for display.

#### 2.1.7. `mcu_timer.cpp` / `.h`

*   **Purpose:** Emulates the hardware timers.
*   **Key Components:**
    *   `TIMER_Clock(int cycles)`: This function is called by the main CPU loop to advance the timers. It uses prescalers to increment the timer counters at the correct rate relative to the CPU clock.
    *   It handles timer overflows and compare-matches, which can trigger interrupts.

#### 2.1.8. `submcu.cpp` / `.h`

*   **Purpose:** Emulates the secondary MCU, which is dedicated to handling MIDI and serial I/O.
*   **Key Components:**
    *   This is a complete, separate CPU emulation core (likely for a 6502-family processor).
    *   It communicates with the main MCU via a shared RAM region and semaphore flags.
    *   It offloads the real-time MIDI processing from the main CPU.

### 2.2. `src/standard` - The Standalone Application & GUI Reference

The `standard` directory contains the source code for the standalone application. Crucially, **this serves as the reference implementation for the VST3 GUI**. The goal is not to build the UI from scratch, but to **translate the logic** from this SDL2 implementation to VSTGUI.

#### 2.2.1. `main.cpp`

*   **Purpose:** The entry point for the standalone application.
*   **Key Components:**
    *   Demonstrates a multi-threaded architecture: a dedicated thread is created for each emulator instance to run the `emu.Step()` loop.
    *   The main thread is responsible only for the GUI event loop (polling SDL for input and rendering). This separation of concerns is a good model to follow for the VST3 plugin to ensure a responsive UI.

#### 2.2.2. `lcd_sdl.cpp` / `.h` - GUI and LCD Implementation Reference

*   **Purpose:** This file is the most critical reference for the VST3 GUI port. It implements the `LCD_Backend` interface using SDL2 and contains all the logic for rendering the user interface and handling user input.
*   **Key Components for Translation:**
    *   **Background Image:** In the `LCD_SDL::Start()` method, it loads the background image from `Data/sc55_background.bmp` using `SDL_LoadBMP`. This exact bitmap should be used as the background for the VST3 plugin's UI.
    *   **Rendering Logic:** The `LCD_SDL::Render()` method is the core rendering loop. It takes the raw pixel data from the emulated LCD (provided by the `backend`) and blits it onto the background texture at the correct coordinates. It then presents the final composed texture to the screen. This composition logic must be replicated in VSTGUI.
    *   **Input Handling:** The `LCD_SDL::HandleEvent()` method contains a large `switch` statement that processes SDL events (mouse clicks, keyboard presses). It checks the coordinates of mouse clicks to determine which on-screen button was pressed and then communicates this to the emulator core by setting bits in the `mcu_t::button_pressed` integer. This entire mapping of coordinates to button actions must be translated to the VSTGUI framework.

#### 2.2.3. `output_sdl.cpp` / `.h`

*   **Purpose:** Implements audio output using the SDL library.
*   **Key Components:**
    *   Implements the SDL audio callback function. This function is called by the SDL audio thread and is responsible for feeding audio samples to the sound card.
    *   It uses a thread-safe ring buffer to transfer audio data from the emulator thread to the SDL audio thread. This producer-consumer pattern is a useful reference, although the VST3 SDK's `process` method will handle this differently.


## 3. VST3 Porting Plan

The existing `backend` library is already well-suited for porting to a VST3 plugin. The use of callbacks and abstract interfaces makes it easy to integrate the emulator into the VST3 framework.

### 3.1. Feasibility Study: Rewrite vs. Re-implement

Given the modular and platform-agnostic design of the `backend` library, a complete rewrite is not necessary. The existing emulation core can be re-used with minimal modifications. The main effort will be to write a new VST3-specific "frontend" that replaces the existing SDL-based frontend.

**Conclusion:** It is more feasible to re-implement the existing `backend` as a VST3 plugin rather than rewriting everything from scratch.

### 3.2. VST3 Plugin Architecture

The VST3 plugin will consist of the following main components:

*   **Processor:** This component will contain the `Emulator` instance and will be responsible for processing audio and MIDI.
*   **Controller:** This component will manage the plugin's state and will communicate with the GUI.
*   **GUI:** This component will be responsible for displaying the plugin's user interface.

### 3.3. Implementation Steps

#### 3.3.1. Create the VST3 Project

1.  Create a new directory `vst/` in the root of the repository.
2.  Set up a new CMake project in the `vst/` directory that uses the Steinberg VST3 SDK.
3.  Link the `backend` library to the VST3 project.

#### 3.3.2. Implement the VST3 Processor

1.  Create a new class that inherits from `Steinberg::Vst::AudioEffect`.
2.  In the constructor of this class, create an instance of the `Emulator` class.
3.  Implement the `process()` method. This method will be called by the DAW to process audio and MIDI.
    *   In the `process()` method, pass the incoming MIDI messages to the `Emulator` instance.
    *   Call the `Emulator::Step()` method to run the emulation.
    *   Use the `SetSampleCallback()` method to get the generated audio samples and write them to the output buffers.
4.  Implement the `setState()` and `getState()` methods to save and restore the plugin's state.

#### 3.3.3. Implement the VST3 GUI

1.  Create a new class that inherits from `Steinberg::Vst::EditController`.
2.  Use the VSTGUI library (which is part of the VST3 SDK) to create the user interface.
3.  **Translate the existing SDL2 GUI:** The UI should be a direct translation of the logic found in `src/standard/lcd_sdl.cpp`.
    *   Load the `Data/sc55_background.bmp` as the main UI background using VSTGUI's `CBitmap` class.
    *   Replicate the rendering logic from `LCD_SDL::Render()` to composite the emulated LCD screen (from the `backend`) onto the background bitmap.
    *   Re-implement the input mapping from `LCD_SDL::HandleEvent()`. Create clickable areas (e.g., using `CTextButton` with transparent textures) that correspond to the button coordinates in the original file and link them to the appropriate actions in the emulator core.
4.  The `LCD_Backend` interface will be implemented within the VSTGUI framework. The `Render` method of this backend will update a VSTGUI `CBitmap` or similar view that represents the LCD screen.
5.  **Future-proofing:** While the initial implementation will be a direct port of the SC-55 GUI, the architecture should allow for easily adding new layouts and background bitmaps for other hardware targets in the future.

#### 3.3.4. MIDI Implementation

*   **MIDI In:** The VST3 plugin will receive MIDI messages from the DAW. These messages will be passed to the `Emulator` instance, which will handle them appropriately.
*   **MIDI Out:** The `Emulator` can also generate MIDI messages. The `SetMidiOutCallback()` method will be used to get these messages and send them to the DAW.

## 4. Future Goals

### 4.1. 100% Translation of Existing Code

The goal is to have a 100% accurate translation of the existing C and C++ emulator code. This means that the VST3 plugin should sound identical to the standalone application.

### 4.2. Flexible GUI Framework

The GUI framework should be designed to make it easy to add new skins for different hardware models. This will involve:

*   A clear separation between the GUI code and the emulation code.
*   A data-driven approach to defining the layout and behavior of the UI controls.
*   The ability to easily swap out background bitmaps and other graphical assets.

### 4.3. Fully Working MIDI Implementation

The MIDI implementation should be robust and should support all the features of the original hardware, including MIDI In, Out, and Thru.
