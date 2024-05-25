
#include <conio.h>
#include <iostream>
#include <Windows.h>
#include <memoryapi.h>

// Function prototypes
void notes();
void menu();
void injectProcess();
void modifyProcess();

// ESC key handler function & thread
DWORD WINAPI monitorESC(LPVOID lpParam);
HANDLE escEvent; 

// Menu threads
HANDLE menuThread[2];


void notes() {
	/*
	 *
	 * Dynamic Loading
	 * ===============
	 *
	 * Dynamic Loading is a technique that allows executable files
	 * to load and unload shared libraries (also known as Dynamic Link Libraries or DLLs)
	 * at runtime.
	 *
	 *
	 * Dynamic Linker
	 * ===============
	 *
	 * A dynamic linker is a component of the operating system's runtime environment
	 * that loads and links shared libraries to executable files. It resolves symbols
	 * (function names, variable names, etc.) from the shared libraries and connects
	 * them with the corresponding symbols in the executable file.
	 *
	 *
	 * DLLs
	 * ===============
	 * DLLs are shared libraries that contain code and data that can be used by multiple
	 * applications. They are loaded into the address space of a process only when needed.
	 *
	 *
	 * DLL Injection
	 * ===============
	 * DLL injection is a technique used to inject a DLL (Dynamic Link Library) into a
	 * running process, allowing the DLL to access and modify the process's memory and behavior
	 *
	 *
	 * ======================
	 * Project Goal & Outline
	 * ======================
	 *
	 * Tool that does the following:
	 *		1. Injects DDLs into a target process (dropdown menu) that monitors
	 *		the modified memory values based on a datatype: int, string, float, etc.
	 *
	 *		   Prints values in the format given
	 *			[Type] : [Previous Value] : [New Value] : [Address]
	 *
	 *
	 *		2. Allows you to read and write to specific addresses in the processes memory
	 *		 - Uses ReadProcessMemory() and WriteProcessMemory() functions
	 * */
}


/*
 * --------------------------------------------------
 *
 *
 * --------------------------------------------------
*/
void modifyProcess() {
	system("cls");
	std::cout << "Modifying process. Press ESC to return to menu..." << std::endl;

	escEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 
	HANDLE escThread = CreateThread(NULL, 0, monitorESC, NULL, 0, NULL);

	// Simulate some processing work
	while (WaitForSingleObject(escEvent, 100) == WAIT_TIMEOUT) {
		// Simulating work by sleeping
	}

	WaitForSingleObject(escThread, INFINITE);
	CloseHandle(escThread);
	CloseHandle(escEvent);
}


/*
 * --------------------------------------------------
 *
 *
 * --------------------------------------------------
*/
void injectProcess() {
	system("cls");
	std::cout << "Injecting process. Press ESC to return to menu..." << std::endl;

	escEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 
	HANDLE escThread = CreateThread(NULL, 0, monitorESC, NULL, 0, NULL);

	// Simulate some processing work
	while (WaitForSingleObject(escEvent, 100) == WAIT_TIMEOUT) {
		// Simulating work by sleeping
	}

	WaitForSingleObject(escThread, INFINITE);
	CloseHandle(escThread);
	CloseHandle(escEvent);
}


/*
 * --------------------------------------------------
 *
 *
 * --------------------------------------------------
*/
DWORD WINAPI monitorESC(LPVOID lpParam) {
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			SetEvent(escEvent);
			ExitThread(0);
		}
		Sleep(100); 
	}
}


/*
 * --------------------------------------------------
 * 
 * 
 * --------------------------------------------------
*/
void menu() {
	// output menu title
	std::cout << R"(
                        _____                           _   
  /\/\   ___ _ __ ___   \_   \_ __  ___ _ __   ___  ___| |_ 
 /    \ / _ \ '_ ` _ \   / /\/ '_ \/ __| '_ \ / _ \/ __| __|
/ /\/\ \  __/ | | | | /\/ /_ | | | \__ \ |_) |  __/ (__| |_ 
\/    \/\___|_| |_| |_\____/ |_| |_|___/ .__/ \___|\___|\__|
                                       |_|                      
	)" << '\n';

	int menu_option = 1;
	
	while (true) {
		system("cls");

		// print menu options
		if (menu_option == 1) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "[1]: DLL Injection\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			std::cout << "[2]: Memory Viewing & Modification\n";
		}
		else {
			std::cout << "[1]: DLL Injection\n";
		
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "[2]: Memory Viewing & Modification\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	
		// handle keyboard events
		while (true) {
			if (GetAsyncKeyState(VK_UP)) {
				// up key pressed
				menu_option = menu_option == 1 ? 2 : menu_option - 1;
				break;
			}
			if (GetAsyncKeyState(VK_DOWN)) {
				// down key pressed
				menu_option = menu_option == 2 ? 1 : menu_option + 1;
				break;
			}
			if (GetAsyncKeyState(VK_RETURN)) {
				// enter key pressed, call appropriate functions
				switch (menu_option) {
					case 1:
						if (menuThread[0] != NULL) {
							CloseHandle(menuThread[0]);
						}
						
						menuThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injectProcess, NULL, 0, NULL);
						WaitForSingleObject(menuThread[0], INFINITE);
						CloseHandle(menuThread[0]);
						menuThread[0] = NULL;
						
						break;
					
					case 2:
						if (menuThread[1] != NULL) {
							CloseHandle(menuThread[1]);
						}
						
						menuThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modifyProcess, NULL, 0, NULL);
						
						WaitForSingleObject(menuThread[1], INFINITE);
						CloseHandle(menuThread[1]);
						menuThread[1] = NULL;
						
						break;
					}
					break;
			}

			Sleep(100);
		}
	}
}


int main()
{
    // display menu
    menu();
    return 0;
}