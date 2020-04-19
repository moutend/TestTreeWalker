#include <windows.h>

#include <strsafe.h>
#include <tlhelp32.h>

#include "util.h"

void SafeCloseHandle(HANDLE *pHandle) {
  if (pHandle != nullptr) {
    CloseHandle(*pHandle);
    *pHandle = nullptr;
  }
}
