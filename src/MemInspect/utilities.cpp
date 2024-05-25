#include "utilities.h"

/*
 * --------------------------------------------------
 * Takes a snapshot of the processes and stores
 * them inside a vector
 * 
 * @return : Returns a vector of the processes
 * --------------------------------------------------
*/
std::vector<std::string> getProcesses() {
    std::vector<std::string> processList;


	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap != INVALID_HANDLE_VALUE) {
		
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		
		if (Process32First(hSnap, &pe32)) {
			do {
				char buffer[1024];
				size_t bufferSize = 1024;
				wcstombs_s(&bufferSize, buffer, 1024, pe32.szExeFile, _TRUNCATE);
				processList.push_back(std::string(buffer));
			} while (Process32Next(hSnap, &pe32));
		}

		CloseHandle(hSnap);
	}

    return processList;
}


/*
 * --------------------------------------------------
 * Gets the process ID from the process name
 *
 * @param name : 
 * --------------------------------------------------
*/
DWORD getProcessId(const std::string& processName) {
    //
    return 0;
}


/*
 * --------------------------------------------------
 * Injects a DLL into a process
 * 
 * @param process : 
 * @param dll     : 
 * --------------------------------------------------
*/
bool injectDll(DWORD process, const std::string& dll) {
    //
    return false;
}


/*
 * --------------------------------------------------
 * Monitors modified memory values in the process
 * 
 * @param process : 
 * @param hModule :
 * --------------------------------------------------
*/
void monitorMemoryValues(DWORD process, HMODULE hModule) {
   // 
}