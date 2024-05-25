#include <map> 
#include <vector>
#include <iterator> 
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;
typedef const wchar_t* str;

/*
 * Winmine Game class
 */
class Winmine
{
	public:
		Winmine();
		~Winmine();
		void ChangeTime();
		void GetTime();

	private:
};

Winmine::Winmine()
{
}

Winmine::~Winmine()
{
}

void Winmine::ChangeTime() 
{
}

void Winmine::GetTime()
{
}



/*
 * Finds the process id of your process
 * Return The PID of the process
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

/*
 * Finds the base address of the target process
 * Return The base address of the process
 */
uintptr_t GetBase(DWORD pid, str process_name)
{
	// Get a list of the running processes.
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	uintptr_t base_address = 0;
	
	if (process_snapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 module_entry;
		module_entry.dwSize = sizeof(module_entry);
		if (Module32First(process_snapshot, &module_entry)) {
			do {
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
 * Return The last address of a multi-level pointer
 */
uintptr_t GetLastAddress(HANDLE process_handle, uintptr_t ptr, std::vector<unsigned int> address_offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < address_offsets.size(); ++i) {
		// Iterate through our offsets, adding each to our final address.
		ReadProcessMemory(process_handle, (BYTE*)address, &address, sizeof(address), 0);
		address += address_offsets[i];
	}

	return address;
}

/*
 * Obtains the id, base address, and handler for the target process
 * Return A map containing the process_handle (key) and module_base (value) of the application.
 */
 std::map<HANDLE, DWORD> Initialize(str process_name)
{
	HANDLE process_handle = 0;
	DWORD process_id = GetPid(process_name);
	DWORD module_base = GetBase(process_id, process_name); 
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

int main()
{
    str application = L"notepad.exe"; 
    wcout << "process name: " << application << endl;
    wcout << "PID: " << GetPid(application) << endl;

    // map of your handle and module_base for the process
	map<HANDLE, DWORD> process_info = Initialize(application);
}