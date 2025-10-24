# Building the Nuked SC-55 VST3 Plugin

This guide provides instructions on how to build the Nuked SC-55 VST3 plugin from source.

## Prerequisites

Before you begin, ensure you have the following software installed:

*   **Git:** For version control.
*   **CMake:** For building the project (version 3.19 or higher).
*   **A C++17 compliant compiler:** Such as GCC, Clang, or MSVC.

### Dependencies

The VST3 plugin relies on several libraries for its user interface, provided by the VSTGUI framework. You will need to have the development packages for these libraries installed on your system. The required libraries are:

*   **X11 / XCB:** A suite of libraries for the X Window System.
*   **fontconfig:** For managing fonts.
*   **freetype:** For font rendering.
*   **glib-2.0:** A core utility library.
*   **cairo:** A 2D graphics library.
*   **pango:** A library for laying out and rendering of text.

Please use your system's package manager (e.g., `apt-get` on Debian/Ubuntu, `brew` on macOS, `vcpkg` on Windows) to install these dependencies.

## Build Steps

1.  **Clone the Repository**

    First, clone the main repository from GitHub:
    ```bash
    git clone https://github.com/linoshkmalayil/Nuked-SC55.git
    cd Nuked-SC55
    ```

2.  **Initialize Submodules**

    The project uses git submodules to manage external dependencies, including the Steinberg VST3 SDK. It's crucial to initialize them correctly.

    If you cloned the repository without the `--recursive` flag, you can initialize the submodules with this command:
    ```bash
    git submodule update --init --recursive
    ```

3.  **Configure the Build with CMake**

    The VST3 plugin has a dedicated build configuration located in the `vst/` directory.

    ```bash
    cd vst
    mkdir build
    cd build
    cmake ..
    ```

4.  **Build the Plugin**

    Once CMake has configured the project, you can build the plugin:
    ```bash
    cmake --build .
    ```
    For a release build, you can specify the configuration:
    ```bash
    cmake --build . --config Release
    ```

## Output

The compiled VST3 plugin, `Nuked SC-55.vst3`, will be located in the `vst/build/VST3/Release/` (or `Debug`) directory. You can then copy this file to your system's VST3 plugin directory (e.g., `~/.vst3/` on Linux, `~/Library/Audio/Plug-Ins/VST3/` on macOS, or `C:\Program Files\Common Files\VST3\` on Windows).
