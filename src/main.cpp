#include <locale.h>
#include <windows.h>

#include "run.h"

int main(Platform::Array<Platform::String ^> ^ args) {
  setlocale(LC_ALL, "Japanese");

  run();

  return 0;
}
