#include <conio.h>
#include <iostream>
#include "utilities.h"

/* Function prototypes & Globals */

HANDLE menuThread[2];

void main_menu();
void injection_menu();
void modification_menu();

void menu_navigation(std::string a, std::string b, std::vector<std::string> c);
void monitorESC(HANDLE& threadHandle);

/*
 * --------------------------------------------------
 * Terminates the calling thread that used ESC key
 * 
 * @param threadHandle : calling thread
 * --------------------------------------------------
*/
void monitorESC(HANDLE& threadHandle) {
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			TerminateThread(threadHandle, 0);
			ExitThread(0);
		}
		Sleep(100);
	}
}

/*
 * --------------------------------------------------
 * Setups menu navigiation for the given selections
 * 
 * @param title		 : title art for menu
 * @param name       : name of menu
 * @param selections : menu selection items
 * --------------------------------------------------
*/
void menu_navigation(std::string title, std::string name, std::vector<std::string> selections) {
	
	int menu_option = 1;
	int count = selections.size();

	// menu keys
	std::vector<int> numberKeys = { '1', '2', '3' }; 

	while (true) {
		// menu display section 
		// (probably shouldn't use a goto statement even though I love them?)
		menu_display: 
			system("cls");

			// print menu title
			std::cout << title << "\n";

			// print menu selection items
			for (int i = 0; i < count; i++) {
				// print selected item 
				if (i+1 == menu_option) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					std::cout << "[" << i+1 << "]: " << selections[i] << "\n";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else {
					// unselected item print
					std::cout << "[" << i+1 << "]: " << selections[i] << "\n";
				}
			}

		// keyboard event handler
		while (true) {
			for (int i = 0; i < numberKeys.size(); i++) {
				if (GetAsyncKeyState(numberKeys[i]) < 0) {
					// number key pressed
					switch (numberKeys[i]) {
						case '1':
							menu_option = 1;
							break;
						case '2':
							menu_option = 2;
							break;
						case '3':
							menu_option = 3;
							break;
					}

					// update menu color:
					goto menu_display;
					break;
				}
			}

			if (GetAsyncKeyState(VK_RETURN) < 0) {
				// enter key pressed, handle various menu types
			
				if (name == "main") {
					// handle selected option
					if (menu_option == 1) {
						// DLL injection and memory viewing
						menuThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injection_menu, NULL, 0, NULL);
						WaitForSingleObject(menuThread[0], INFINITE);
						CloseHandle(menuThread[0]);
						break;
					}
					else {
						// Memory reading and writing
						menuThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modification_menu, NULL, 0, NULL);
						WaitForSingleObject(menuThread[1], INFINITE);
						CloseHandle(menuThread[1]);
						break;
					}
				}
				if (name == "injection") {
					// handle selected option
					if (menu_option == 1) {
						std::cout << "Output running processes list\n";
						// std::vector<std::string> processList = getProcesses();
					}
					else if (menu_option == 2) {
						std::cout << "Inject DLL into a process\n";
					}
					else {
						std::cout << "Monitor modified memory value\n";
					}
				}
				if (name == "modification") {
					// handle selected option
					if (menu_option == 1) {
						// Read the value of an addresses in the processes memory 
						std::cout << "About to read a value in memory\n";
					}
					else {
						// Write to an addresses in the processes memory
						std::cout << "About to write to an address\n";
					}
				}

				break;
			}

			Sleep(100);
		}

	}
}

/*
 * --------------------------------------------------
 *    Setups the modification menu for the program
 * --------------------------------------------------
*/
void modification_menu() {
	// create inject process menu esc thread (press esc key)
	HANDLE escThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)monitorESC, &menuThread[1], 0, NULL);


	// output injection menu title
	std::string title = R"(
                  _ _  __ _           _   _             
  /\/\   ___   __| (_)/ _(_) ___ __ _| |_(_) ___  _ __  
 /    \ / _ \ / _` | | |_| |/ __/ _` | __| |/ _ \| '_ \ 
/ /\/\ \ (_) | (_| | |  _| | (_| (_| | |_| | (_) | | | |
\/    \/\___/ \__,_|_|_| |_|\___\__,_|\__|_|\___/|_| |_|
                                                                                                 
	)";

	// setup menu selection items
	std::vector<std::string> selections = {
		"Read the value of an address in the processes memory",
		"Write to an address in the processes memory"
	};

	// setup desired menu
	menu_navigation(title, "modification", selections);

	// handle esc thread termination
	WaitForSingleObject(escThread, INFINITE);
	CloseHandle(escThread);
}

/*
 * --------------------------------------------------
 *    Setups the injection menu for the program
 * --------------------------------------------------
*/
void injection_menu() {
	// create inject process menu esc thread (press esc key)
	HANDLE escThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)monitorESC, &menuThread[0], 0, NULL);


	// output injection menu title
	std::string title = R"(
  _____        _           _   _             
  \_   \_ __  (_) ___  ___| |_(_) ___  _ __  
   / /\/ '_ \ | |/ _ \/ __| __| |/ _ \| '_ \ 
/\/ /_ | | | || |  __/ (__| |_| | (_) | | | |
\____/ |_| |_|/ |\___|\___|\__|_|\___/|_| |_|
            |__/                                              
	)";

	// setup menu selection items
	std::vector<std::string> selections = {
		"View Processes",
		"Inject DLL into a process",
		"Monitor modified memory values" 
	};

	// setup desired menu
	menu_navigation(title, "injection", selections);

	// handle esc thread termination
	WaitForSingleObject(escThread, INFINITE);
	CloseHandle(escThread);
}

/*
 * --------------------------------------------------
 *        Setups the main menu for program
 * --------------------------------------------------
*/
void main_menu() {
	// output main menu title
	std::string title = R"(
                        _____                           _   
  /\/\   ___ _ __ ___   \_   \_ __  ___ _ __   ___  ___| |_ 
 /    \ / _ \ '_ ` _ \   / /\/ '_ \/ __| '_ \ / _ \/ __| __|
/ /\/\ \  __/ | | | | /\/ /_ | | | \__ \ |_) |  __/ (__| |_ 
\/    \/\___|_| |_| |_\____/ |_| |_|___/ .__/ \___|\___|\__|
                                       |_|                      
	)";

	// setup menu selection items
	std::vector<std::string> selections = {
		"DLL Injection",
		"Memory Viewing & Modification"
	};

	// setup desired menu
	menu_navigation(title, "main", selections);
}

// main entry point
int main()
{
    // calls starting menu
	main_menu();
    return 0;
}

/*
 *										Notes:
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