#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllexport)
DWORD WINAPI MessageBoxThread(LPVOID lpParam) {
  MessageBox(0, "Hello world!", "@MemInspect!", 0);
  return 0;
}

extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      MessageBox(0, "Hello world!", "@MemInspect!", 0);
      break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}