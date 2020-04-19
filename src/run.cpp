#include <UIAutomationClient.h>
#include <iostream>

#include "run.h"
#include "util.h"

HRESULT run() {
  HRESULT hr{};

  IUIAutomation5 *pUIAutomation{};
  IUIAutomationCondition *pCondition{};
  IUIAutomationElement *pRootElement{};
  IUIAutomationElementArray *pFound{};

  hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  if (FAILED(hr)) {
    return hr;
  }

  hr = CoCreateInstance(__uuidof(CUIAutomation8), nullptr, CLSCTX_INPROC_SERVER,
                        __uuidof(IUIAutomation5),
                        reinterpret_cast<void **>(&pUIAutomation));

  if (FAILED(hr)) {
    return hr;
  }

  hr = pUIAutomation->CreateTrueCondition(&pCondition);

  if (FAILED(hr)) {
    goto CLEANUP;
  }

  Sleep(8000);

  HWND hForegroundWindow = GetForegroundWindow();

  hr = pUIAutomation->ElementFromHandle(hForegroundWindow, &pRootElement);

  if (FAILED(hr)) {
    goto CLEANUP;
  }

  hr = pRootElement->FindAll(TreeScope_Subtree, pCondition, &pFound);

  if (FAILED(hr)) {
    goto CLEANUP;
  }

  int length{};

  hr = pFound->get_Length(&length);

  if (FAILED(hr)) {
    goto CLEANUP;
  }

  wprintf(L"Found %d item(s)\n", length);

  for (int i = 0; i < length; i++) {
    IUIAutomationElement *pElement{};

    hr = pFound->GetElement(i, &pElement);

    if (FAILED(hr)) {
      wprintf(L"%d. Failed to retrieve element.\n", i + 1);

      continue;
    }

    BSTR name;

    hr = pElement->get_CurrentName(&name);

    if (FAILED(hr)) {
      wprintf(L"%d. Failed to get name.\n", i + 1);

      goto RELEASE;
    }

    wprintf(L"%d. Name=\"%s\"\n", i + 1, name);

  RELEASE:

    if (name != nullptr) {
      SysFreeString(name);
    }

    SafeRelease(&pElement);
  }
CLEANUP:

  SafeRelease(&pFound);
  SafeRelease(&pRootElement);
  SafeRelease(&pCondition);
  SafeRelease(&pUIAutomation);

  CoUninitialize();

  return hr;
}
