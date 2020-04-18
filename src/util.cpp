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

bool IsEmptyIUIAutomationElement(IUIAutomationElement *pElement) {
  if (pElement == nullptr) {
    return true;
  }

  RECT boundingRectangle{0, 0, 0, 0};

  if (FAILED(pElement->get_CurrentBoundingRectangle(&boundingRectangle))) {
    return true;
  }
  if (boundingRectangle.left == 0 && boundingRectangle.top == 0 &&
      boundingRectangle.right == 0 && boundingRectangle.bottom == 0) {
    return true;
  }

  wchar_t *currentName{nullptr};
  wchar_t *cachedName{nullptr};

  if (FAILED(pElement->get_CurrentName(&currentName))) {
    currentName = nullptr;
  }
  if (FAILED(pElement->get_CachedName(&cachedName))) {
    cachedName = nullptr;
  }
  if (currentName == nullptr && cachedName == nullptr) {
    return true;
  }
  if (currentName != nullptr && std::wcslen(currentName) == 0) {
    return true;
  }
  if (cachedName != nullptr && std::wcslen(cachedName) == 0) {
    return true;
  }
  if (cachedName != nullptr) {
    SysFreeString(cachedName);
    cachedName = nullptr;
  }
  if (currentName != nullptr) {
    SysFreeString(currentName);
    currentName = nullptr;
  }

  return false;
}
