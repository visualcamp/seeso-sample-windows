#ifndef WindowDisplayUtil_h
#define WindowDisplayUtil_h

#include <string>
#include <vector>

#include <windows.h>
#pragma comment(lib, "user32.lib")

namespace seeso {


struct DisplayInfo {
  std::string displayName;
  std::string displayString;
  int displayStateFlag;
  std::string displayId;
  std::string displayKey;
  int widthMm;
  int heightMm;
  int widthPx;
  int heightPx;
};

std::vector<DisplayInfo> getDisplayLists();

class WindowsDisplayUtil {
 public:
  bool init();

  const DisplayInfo& getDisplay() const;
  DisplayInfo& getDisplay();

 private:
  DisplayInfo display;
};

} // namespace seeso

#endif // WindowDisplayUtil_h
