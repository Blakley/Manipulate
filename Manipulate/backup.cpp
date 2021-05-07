#include <map> 
#include <vector>
#include <iterator> 
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

// short-hand for wchar_t*
typedef const wchar_t* wstring;

/*
 * Finds the process id of your process
 *
 * @param process_name The name of the process
 * @return The process-id of the process
 */
DWORD GetProcessID(wstring process_name)
{
	// Debugger Info
	std::cout << "Entering GetProcessID()" << std::endl;
	std::cout << std::endl;

	// Get a list of the running processes.
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD proccess_id = 0;

	if (process_snapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(process_entry);

		// Iterate through the processes to find the target.
		if (Process32First(process_snapshot, &process_entry))
		{
			do
			{
				// Compare the current process with the target process.
				if (!_wcsicmp(process_entry.szExeFile, process_name))
				{
					proccess_id = process_entry.th32ProcessID;
					break;
				}

			} while (Process32Next(process_snapshot, &process_entry));
		}
	}

	CloseHandle(process_snapshot);
	return proccess_id;
}

/*
 * Finds the base address of the target process
 *
 * @param process_id ID of the target process
 * @param process_name The name of the process
 * @return The base address of the process
 */
uintptr_t GetBaseAddress(DWORD process_id, wstring process_name)
{
	// Debug info
	std::cout << "Entering GetBaseAddress() " << std::endl;
	std::cout << std::endl;

	// Get a list of the running processes.
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	uintptr_t base_address = 0;
	
	if (process_snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 module_entry;
		module_entry.dwSize = sizeof(module_entry);
		if (Module32First(process_snapshot, &module_entry))
		{
			do
			{
				if (!_wcsicmp(module_entry.szModule, process_name))
				{
					base_address = (uintptr_t)module_entry.modBaseAddr;
					break;
				}
			} while (Module32Next(process_snapshot, &module_entry));
		}
	}

	CloseHandle(process_snapshot);
	return base_address;
}

/*
 * Gets the last address for the supplied pointer
 *
 * @param process_handle Pointer to the process
 * @param address_offsets A list of address offsets of the pointer
 * @return The last address of a multi-level pointer
 */
uintptr_t GetLastAddress(HANDLE process_handle, uintptr_t ptr, std::vector<unsigned int> address_offsets)
{
	std::cout << "Entering FindLastAddress()" << std::endl;
	std::cout << std::endl;

	uintptr_t address = ptr;
	for (unsigned int i = 0; i < address_offsets.size(); ++i)
	{
		// Iterate through our offsets, adding each to our final address.
		ReadProcessMemory(process_handle, (BYTE*)address, &address, sizeof(address), 0);
		address += address_offsets[i];
	}

	return address;
}

/*
 * Obtains the id, base address, and handler for the target process
 * 
 * @param application_name The name of the application you're interacting with.
 * @return A map containing the process_handle (key) and module_base (value) of the application.
 */
 std::map<HANDLE, DWORD> Initialize(wstring process_name)
{
	 // Debug info
	 std::cout << "Entering Initialize()" << std::endl;
	 std::cout << std::endl;

	HANDLE process_handle = 0;
	DWORD process_id = GetProcessID(process_name);
	DWORD module_base = GetBaseAddress(process_id, process_name); 
	process_handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);

	std::map<HANDLE, DWORD> mp;
	mp.insert({ process_handle, module_base });

	std::cout << "process_handle: " << process_handle << std::endl;
	std::wcout << "process_name: " << process_name << std::endl;
	std::cout << "process_id: " << process_id << std::endl;
	std::cout << "module_base: " << module_base << std::endl;
	std::cout << std::endl;

	return mp;
}

 /*
  * Changes the time counter in MineSweeper 
  * Example for writing to a process
  *
  * @param info Map containing the handle and module_base
  * @param new_time The time you wish to have
  */
 int ChangeTime(std::map<HANDLE, DWORD> info, int new_time)
 {
	// Get our dynamic base address for the time
	HANDLE p_handle = info.begin()->first; // process handle
	DWORD module_base = info.begin()->second; // base addr of module
	uintptr_t dynamic_base_address_time = module_base + 0x579C; // 0x579C is the base address found.

	// offsets for the address (Our base address we found is static so it has no offsets)
	std::vector<unsigned int> time_offsets = {}; // ie:  = {0x3B8, 0x78, ..}

	// Find the final address of the time var
	uintptr_t time_address = GetLastAddress(p_handle, dynamic_base_address_time, time_offsets);

	// Write our new time to the process
	WriteProcessMemory(p_handle, (BYTE*)time_address, &new_time, sizeof(new_time), nullptr);

	return  new_time;
 }

/*
 * Gets the time counter in MineSweeper
 * Example for reading a process
 *
 * @param info Map containing the handle and module_base
 * @return the current time in MineSweeper
 */
 int GetTime(std::map<HANDLE, DWORD> info)
 {
	 // Get our dynamic base address for the time
	 HANDLE p_handle = info.begin()->first; // process handle
	 DWORD module_base = info.begin()->second; // base addr of module
	 uintptr_t dynamic_base_address_time = module_base + 0x579C; // 0x579C is the base address found.

	 // offsets for the address (Our base address we found is static so it has no offsets)
	 std::vector<unsigned int> time_offsets = {}; // ie:  = {0x3B8, 0x78, ..}

	 // Find the final address of the time var
	 uintptr_t time_address = GetLastAddress(p_handle, dynamic_base_address_time, time_offsets);

	 int current_time = 0; // store our time from MineSweeper
	 // You can also loop reading the process until keyboard input to constantly get the time
	 ReadProcessMemory(p_handle, (BYTE*)time_address, &current_time, sizeof(current_time), nullptr);

	 return current_time;
 }

int main(void)
{
	// your process name
	wstring process_name = L"Winmine__XP.exe";

	// map of your handle and module_base for the process
	std::map<HANDLE, DWORD> process_info = Initialize(process_name);

	// After starting, get the time from MinesSweeper
	int time = GetTime(process_info); 
	std::cout << "Current time: " << time << std::endl;
	std::cout << std::endl;

	// change the time in MineSweeper
	int new_time = 30; // 30 seconds
	int time_now = ChangeTime(process_info, new_time);
	std::cout << "New Time: " << time_now << std::endl;
	std::cout << std::endl;

	system("pause");
    return 0;
}
