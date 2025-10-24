# Building the Nuked SC-55 VST3 Plugin

This guide provides instructions on how to build the Nuked SC-55 VST3 plugin from source on a Debian-based Linux distribution like Ubuntu.

## Prerequisites

Before you begin, you will need to install the following dependencies:

*   **Git, CMake, and a C++ compiler:**
    ```bash
    sudo apt-get update
    sudo apt-get install -y git cmake build-essential
    ```

*   **X11/XCB libraries:** VSTGUI requires a number of X11 and XCB libraries for its user interface. You can install them all with the following command:
    ```bash
    sudo apt-get install -y libxcb1-dev libxkbcommon-dev libxcb-util0-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-render-util0-dev libxcb-xinerama0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-glx0-dev libxkbcommon-x11-dev libxcb-cursor-dev libxcb-ewmh-dev
    ```

*   **Other UI libraries:** VSTGUI also depends on a few other libraries for UI rendering and font handling.
    ```bash
    sudo apt-get install -y libglib2.0-dev libcairo2-dev libpango1.0-dev
    ```

## Build Steps

1.  **Clone the repository:**
    ```bash
    git clone --recursive https://github.com/linoshkmalayil/Nuked-SC55.git
    ```

2.  **Navigate to the VST directory:**
    ```bash
    cd Nuked-SC55/vst
    ```

3.  **Create a build directory:**
    ```bash
    mkdir build && cd build
    ```

4.  **Configure the build with CMake:**
    ```bash
    cmake ..
    ```

5.  **Build the plugin:**
    ```bash
    cmake --build .
    ```

## Output

The compiled VST3 plugin, `Nuked SC-55.vst3`, will be located in the `vst/build/VST3/` directory. You can then copy this file to your VST3 plugin directory (e.g., `~/.vst3/`).
