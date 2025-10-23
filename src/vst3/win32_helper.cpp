#include "win32_helper.h"
#include <windows.h>
#include <shlobj.h>

namespace VST3 {
namespace Win32 {

static const wchar_t* REGISTRY_KEY = L"Software\\NukedSC55";
static const wchar_t* REGISTRY_VALUE = L"ROMPath";

std::filesystem::path getRomPathFromRegistry()
{
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, REGISTRY_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        wchar_t buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);
        if (RegQueryValueExW(hKey, REGISTRY_VALUE, nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::filesystem::path(buffer);
        }
        RegCloseKey(hKey);
    }
    return "";
}

void setRomPathInRegistry(const std::filesystem::path& path)
{
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, REGISTRY_KEY, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, REGISTRY_VALUE, 0, REG_SZ, (const BYTE*)path.c_str(), (path.native().size() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }
}

std::filesystem::path openFolderDialog()
{
    BROWSEINFOW bi = { 0 };
    bi.lpszTitle = L"Please select the folder where the ROM files are located.";
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != nullptr) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            CoTaskMemFree(pidl);
            return std::filesystem::path(path);
        }
        CoTaskMemFree(pidl);
    }
    return "";
}

void showErrorMessage(const std::string& message)
{
    MessageBoxA(nullptr, message.c_str(), "Nuked SC-55", MB_OK | MB_ICONERROR);
}

} // namespace Win32
} // namespace VST3
