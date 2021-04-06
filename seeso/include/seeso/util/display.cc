
#include "display.h"
//#include "WinUser.h"

#include "wtypes.h"
#include "tchar.h"

#include <memory>
#include <type_traits>
#include <iostream>

#define SEP '\\'
#define SEP_STR "\\"

namespace seeso {

static bool GetMonitorSizeFromEDID(DISPLAY_DEVICE &ddMon, DWORD &Width, DWORD &Height);
static void DumpDevice(const DISPLAY_DEVICE &dd, size_t nSpaceCount);


std::vector<DisplayInfo> getDisplayLists() {
  std::vector<DisplayInfo> displays;

  DISPLAY_DEVICE dd;
  ZeroMemory(&dd, sizeof(dd));
  dd.cb = sizeof(DISPLAY_DEVICE);

  DWORD deviceNum = 0;
  while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {
    DISPLAY_DEVICE newdd;
    ZeroMemory(&newdd, sizeof(newdd));
    newdd.cb = sizeof(DISPLAY_DEVICE);

    DWORD monitorNum = 0;
    while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0)) {

      DWORD mWidth = 0;
      DWORD mHeight = 0;
      if (GetMonitorSizeFromEDID(newdd, mWidth, mHeight)) {
        // obtain width, height
        DisplayInfo display;
        display.displayName = std::string(newdd.DeviceName);
        display.displayString = std::string(newdd.DeviceString);
        display.displayStateFlag = static_cast<int>(newdd.StateFlags);
        display.displayId = std::string(newdd.DeviceID);
        display.displayKey = std::string(newdd.DeviceKey + 42);
        // cm -> mm
        display.widthMm = static_cast<int>(mWidth) * 10;
        display.heightMm = static_cast<int>(mHeight) * 10;

        // primary monitor's screen resolution
        DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
        DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
        display.widthPx = static_cast<int>(dwWidth);
        display.heightPx = static_cast<int>(dwHeight);

        displays.emplace_back(std::move(display));
      }
      monitorNum++;
    }
    deviceNum++;
  }

  return displays;
}

bool WindowsDisplayUtil::init() {
  DISPLAY_DEVICE dd;
  ZeroMemory(&dd, sizeof(dd));
  dd.cb = sizeof(DISPLAY_DEVICE);

  DWORD deviceNum = 0;
  bool isFoundOneMonitor = false;
  while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {

    if (isFoundOneMonitor) {
      break;
    }
    DISPLAY_DEVICE newdd;
    ZeroMemory(&newdd, sizeof(newdd));
    newdd.cb = sizeof(DISPLAY_DEVICE);

    DWORD monitorNum = 0;
    while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0)) {
//            DumpDevice( newdd, 4 );

      DWORD mWidth = 0;
      DWORD mHeight = 0;
      if (GetMonitorSizeFromEDID(newdd, mWidth, mHeight)) {
        // obtain width, height
        DisplayInfo windowsDisplay;
        windowsDisplay.displayName = std::string(newdd.DeviceName);
        windowsDisplay.displayString = std::string(newdd.DeviceString);
        windowsDisplay.displayStateFlag = static_cast<int>(newdd.StateFlags);
        windowsDisplay.displayId = std::string(newdd.DeviceID);
        windowsDisplay.displayKey = std::string(newdd.DeviceKey + 42);
        // cm -> mm
        windowsDisplay.widthMm = static_cast<int>(mWidth) * 10;
        windowsDisplay.heightMm = static_cast<int>(mHeight) * 10;

        // primary monitor's screen resolution
        DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
        DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
        windowsDisplay.widthPx = static_cast<int>(dwWidth);
        windowsDisplay.heightPx = static_cast<int>(dwHeight);

        display = windowsDisplay;
        isFoundOneMonitor = true;
      }
      if (isFoundOneMonitor) {
        break;
      }
      monitorNum++;
    }
    deviceNum++;
  }
  return isFoundOneMonitor;
}

std::pair<long, long> WindowsDisplayUtil::getWindowPosition() {
    long x, y = 0;
    HWND handle = FindWindowA(NULL, "camera");
    RECT rect;
    if( GetWindowRect( handle, &rect ) ) {
        //std::cout << "in " << rect.left << " " << rect.top << " " << rect.right << " " << rect.bottom << std::endl;
        x = rect.left;
        y = rect.top;
    }
    return {x, y};
}
const DisplayInfo& WindowsDisplayUtil::getDisplay() const {
  return display;
}

DisplayInfo& WindowsDisplayUtil::getDisplay() {
  return display;
}

    bool GetMonitorSizeFromEDID(DISPLAY_DEVICE &ddMon, DWORD &Width, DWORD &Height) {
//    DISPLAY_DEVICE ddMon;
//    ZeroMemory(&ddMon, sizeof(ddMon));
//    ddMon.cb = sizeof(ddMon);

  //read edid
  bool result = false;
  Width = 0;
  Height = 0;
  TCHAR model[8];
  TCHAR *s = _tcschr(ddMon.DeviceID, SEP) + 1;
  size_t len = _tcschr(s, SEP) - s;
  if (len >= _countof(model))
    len = _countof(model) - 1;
  _tcsncpy_s(model, s, len);

  TCHAR *path = _tcschr(ddMon.DeviceID, SEP) + 1;
  TCHAR str[MAX_PATH] = _T("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\");
  _tcsncat_s(str, path, _tcschr(path, SEP) - path);
  path = _tcschr(path, SEP) + 1;
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, str, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
    DWORD i = 0;
    DWORD size = MAX_PATH;
    FILETIME ft;
    while (RegEnumKeyEx(hKey, i, str, &size, NULL, NULL, NULL, &ft) == ERROR_SUCCESS) {
      HKEY hKey2;
      if (RegOpenKeyEx(hKey, str, 0, KEY_READ, &hKey2) == ERROR_SUCCESS) {
        size = MAX_PATH;
        if (RegQueryValueEx(hKey2, _T("Driver"), NULL, NULL, (LPBYTE) &str, &size) == ERROR_SUCCESS) {
          if (_tcscmp(str, path) == 0) {
            HKEY hKey3;
            if (RegOpenKeyEx(hKey2, _T("Device Parameters"), 0, KEY_READ, &hKey3) == ERROR_SUCCESS) {
              BYTE EDID[256];
              size = 256;
              if (RegQueryValueEx(hKey3, _T("EDID"), NULL, NULL, (LPBYTE) &EDID, &size) == ERROR_SUCCESS) {
                DWORD p = 8;
                TCHAR model2[9];

                char byte1 = EDID[p];
                char byte2 = EDID[p + 1];
                model2[0] = ((byte1 & 0x7C) >> 2) + 64;
                model2[1] = ((byte1 & 3) << 3) + ((byte2 & 0xE0) >> 5) + 64;
                model2[2] = (byte2 & 0x1F) + 64;
                _stprintf(model2 + 3, _T("%X%X%X%X"), (EDID[p + 3] & 0xf0) >> 4, EDID[p + 3] & 0xf,
                          (EDID[p + 2] & 0xf0) >> 4, EDID[p + 2] & 0x0f);
                if (_tcscmp(model, model2) == 0) {
                  Width = EDID[21];
                  Height = EDID[22];
                  result = true;
                } else {
                  // EDID incorrect
                }
              }
              RegCloseKey(hKey3);
            }
          }
        }
        RegCloseKey(hKey2);
      }
      i++;
    }
    RegCloseKey(hKey);
  }
  return result;
}


void DumpDevice(const DISPLAY_DEVICE &dd, size_t nSpaceCount) {
  printf("%*sDevice Name: %s\n", nSpaceCount, "", dd.DeviceName);
  printf("%*sDevice String: %s\n", nSpaceCount, "", dd.DeviceString);
  printf("%*sState Flags: %x\n", nSpaceCount, "", dd.StateFlags);
  printf("%*sDeviceID: %s\n", nSpaceCount, "", dd.DeviceID);
  printf("%*sDeviceKey: ...%s\n\n", nSpaceCount, "", dd.DeviceKey + 42);
}

}
