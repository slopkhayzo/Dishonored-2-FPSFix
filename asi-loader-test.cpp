#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>

#include <cstdio>
#include <cstring>
#include <cwchar>

namespace {

bool BuildSiblingPath(const wchar_t* filename, wchar_t (&output)[MAX_PATH]) {
    const DWORD length = GetModuleFileNameW(nullptr, output, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return false;
    }

    wchar_t* slash = wcsrchr(output, L'\\');
    if (slash == nullptr) {
        return false;
    }
    slash[1] = L'\0';
    return wcscat_s(output, filename) == 0;
}

bool FileContains(const wchar_t* path, const char* expectedText) {
    HANDLE file = CreateFileW(path, GENERIC_READ,
                              FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                              nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    char contents[4096]{};
    DWORD bytesRead = 0;
    const bool readSucceeded =
        ReadFile(file, contents, sizeof(contents) - 1, &bytesRead, nullptr) != FALSE;
    CloseHandle(file);
    return readSucceeded && strstr(contents, expectedText) != nullptr;
}

}  // namespace

int wmain() {
    wchar_t logPath[MAX_PATH]{};
    if (!BuildSiblingPath(L"d2-high-fps-fix.log", logPath)) {
        std::fwprintf(stderr, L"Unable to derive the integration-test log path.\n");
        return 2;
    }

    IDirectInput8W* directInput = nullptr;
    const HRESULT result = DirectInput8Create(
        GetModuleHandleW(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8W,
        reinterpret_cast<void**>(&directInput), nullptr);
    if (FAILED(result) || directInput == nullptr) {
        std::fwprintf(stderr, L"ASI loader DirectInput forwarding failed: 0x%08lX\n",
                      static_cast<unsigned long>(result));
        return 3;
    }
    directInput->Release();

    bool pluginLoaded = false;
    bool diagnosticWritten = false;
    for (unsigned int attempt = 0; attempt < 100; ++attempt) {
        pluginLoaded =
            GetModuleHandleW(L"Dishonored2HighFPSFix.asi") != nullptr;
        diagnosticWritten =
            FileContains(logPath, "Host executable is not Dishonored2.exe");
        if (pluginLoaded && diagnosticWritten) {
            break;
        }
        Sleep(50);
    }

    if (!pluginLoaded) {
        std::fwprintf(stderr, L"The external ASI loader did not load the plugin.\n");
        return 4;
    }
    if (!diagnosticWritten) {
        std::fwprintf(stderr,
                      L"The plugin did not write its unsupported-host diagnostic.\n");
        return 5;
    }

    std::wprintf(L"External-loader discovery, DirectInput forwarding, plugin load, "
                 L"and fail-closed checks passed.\n");
    return 0;
}
