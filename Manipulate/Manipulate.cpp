#include <map> 
#include <vector>
#include <iterator> 
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

typedef const wchar_t* str;

int main()
{
    str application = L"Anthony"; 
    wcout << "process name: " << application << endl;
}
