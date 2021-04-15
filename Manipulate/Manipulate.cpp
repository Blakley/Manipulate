#include <map> 
#include <vector>
#include <iterator> 
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;
typedef const wchar_t* str;


/*
 * Finds the process id of your process
 *
 * @param process_name The name of the process
 * @return The PID of the process
 */
DWORD GetPid(str process_name)
{
	// Get a list of the running processes.
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD proccess_id = 0;

	if (process_snapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(process_entry);

		// Iterate through the processes to find the target.
		if (Process32First(process_snapshot, &process_entry)) {
			do
			{
				// Compare the current process with the target process.
				if (!_wcsicmp(process_entry.szExeFile, process_name)) {
					proccess_id = process_entry.th32ProcessID;
					break;
				}

			} while (Process32Next(process_snapshot, &process_entry));
		}
	}

	CloseHandle(process_snapshot);
	return proccess_id;
}

int main()
{
    str application = L"notepad.exe"; 
    wcout << "process name: " << application << endl;
    wcout << "PID: " << GetPid(application) << endl;
}
