#include "utilities.h"

/*
 * --------------------------------------------------
 * Takes a snapshot of the processes and stores
 * them inside a vector
 * 
 * @return : Returns a vector of the processes
 * --------------------------------------------------
*/
std::vector<process_info> getProcesses() {
    std::vector<process_info> processList;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap != INVALID_HANDLE_VALUE) {
		
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		
		if (Process32First(hSnap, &pe32)) {
			do {
				// get process name
				char buffer[1024];
				size_t bufferSize = 1024;
				wcstombs_s(&bufferSize, buffer, 1024, pe32.szExeFile, _TRUNCATE);
				
				// store process information
				process_info _info;
				_info.name = std::string(buffer);
				_info.pid = pe32.th32ProcessID;

				processList.push_back(_info);

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