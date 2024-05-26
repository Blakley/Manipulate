#include "utilities.h"



/*
 * --------------------------------------------------
 * Display a file dialog for a DLL
 * 
 * @return : Returns the selected file path
 * --------------------------------------------------
*/
std::string open_file_dialog() {
    OPENFILENAMEW ofn;
    WCHAR szFile[260] = { 0 };
    std::string file_path;

    // Initialize OPENFILENAMEW
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"DLL Files (*.dll)\0*.dll\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

    // display the Open File dialog and retrieve the file path
    if (GetOpenFileNameW(&ofn)) {
        std::wstring ws(szFile); 
        file_path = std::string(ws.begin(), ws.end()); 
    }

    return file_path;
}
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
 * Injects a DLL into a process
 * 
 * @param process : PID for the process
 * @param dll     : DLL path
 * --------------------------------------------------
*/
bool injectDll(DWORD process, std::string dll) {
    const char* dll_path = dll.c_str();

    // get a handle
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, process);

    if (hProc && hProc != INVALID_HANDLE_VALUE) {
        // allocate memory in the process
        void* location = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProc, location, dll_path, strlen(dll_path) + 1, 0);
        
        // write the path to memory, create a remote thread in the process that calls LoadLibraryA which takes in our dll path
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, location, 0, 0);
        
        if (hThread)
            CloseHandle(hThread);
  
        if (hProc)
            CloseHandle(hProc);

        return true;
    }

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