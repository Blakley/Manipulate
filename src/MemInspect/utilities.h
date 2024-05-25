#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

#include <Windows.h>
#include <TlHelp32.h>
#include <memoryapi.h>

// process's information
typedef struct process_info {
	std::string name;
	DWORD pid;
} process_info;


// Returns a list of processes
std::vector<process_info> getProcesses();

// Get the process ID from the process name
DWORD getProcessId(const std::string& processName);

// Inject a DLL into a process
bool injectDll(DWORD processId, const std::string& dllPath);

// Monitor modified memory values
void monitorMemoryValues(DWORD processId, HMODULE hModule);

#endif 