#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <locale>
#include <vector>

#include <Windows.h>
#include <TlHelp32.h>
#include <memoryapi.h>

// process's information
typedef struct process_info {
	std::string name;
	DWORD pid;
} process_info;

// Returns the path of a DLL
std::string open_file_dialog();

// Returns a list of processes
std::vector<process_info> getProcesses();

// Inject a DLL into a process
bool injectDll(DWORD process, std::string dll);

// Monitor modified memory values
void monitorMemoryValues(DWORD processId, HMODULE hModule);

#endif 