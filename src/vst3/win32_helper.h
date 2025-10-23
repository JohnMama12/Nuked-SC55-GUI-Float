#pragma once

#include <string>
#include <filesystem>

namespace VST3 {
namespace Win32 {

std::filesystem::path getRomPathFromRegistry();
void setRomPathInRegistry(const std::filesystem::path& path);
std::filesystem::path openFolderDialog();
void showErrorMessage(const std::string& message);

} // namespace Win32
} // namespace VST3
