#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstdlib>

using namespace std;

namespace fs = filesystem;

bool isValidFilePath(const string& path) {
	fs::path filePath(path);
	// Check if the path is empty
	if (filePath.empty()) {
		return false;
	}
	// Check if the path exists
	if (!fs::exists(filePath)) {
		return false;
	}
	// Check if it's a regular file
	if (!fs::is_regular_file(filePath)) {
		return false;
	}
	return true;
}

void getLastErrorMessage()
{
	DWORD errorCode = GetLastError();
	char errorMessage[256];
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errorMessage,
		sizeof(errorMessage),
		NULL
	);

	cerr << endl << string("Error code: ") + to_string(errorCode) + " - " + errorMessage << "\n\n";

	exit(1);
}

void createProcess(char *command)
{

	string fullCommand = "cmd /c " + string(command);


	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(
		NULL,   // No module name (use command line)
		(LPSTR)fullCommand.c_str(),							// Command line
		NULL,								// Process handle not inheritable
		NULL,								// Thread handle not inheritable
		FALSE,								// Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,									// No creation flags
		NULL,								// Use parent's environment block
		NULL,								// Use parent's starting directory 
		&si,								// Pointer to STARTUPINFO structure
		&pi)								// Pointer to PROCESS_INFORMATION structure
	)
	{
		printf("[!] CreateProcess failed.\n");
		getLastErrorMessage();
	}

	printf("[+] Created Process : %d\n\n", pi.dwProcessId);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

string getConfigPath()
{
	char* libvar;
	string sLibvar;
	size_t requiredSize;
	getenv_s(&requiredSize, NULL, 0, "PROC_LAUNCHER_CONFIGS");
	if (requiredSize == 0)
	{
		printf("[!] PROC_LAUNCHER_CONFIGS doesn't exist!\n");
		exit(1);
	}
	libvar = (char*)malloc(requiredSize * sizeof(char));
	if (!libvar)
	{
		printf("[!] Failed to allocate memory!\n");
		exit(1);
	}
	// Get the value of the LIB environment variable.
	getenv_s(&requiredSize, libvar, requiredSize, "PROC_LAUNCHER_CONFIGS");
	sLibvar = libvar;
	free(libvar);
	return sLibvar;
}

int main(int argc, char *argv[]) {

	bool isShowArgFound = false;
	string dynamicConfigPath;
	bool dynamicConfigPathFound = false;
	string configName;
	bool configNameFound = true;

	if (argc <= 1) {
		printf("[+] Usage: %s path_to_config_file\n", argv[0]);
		return 1;
	}
	else {
		for (int i = 1; i < argc; i++)
		{
			if (strncmp(argv[i], "-p", 2) == 0 && i + 1 < argc) {
				dynamicConfigPath = argv[i + 1];
				dynamicConfigPathFound = true;
			}

			if (strncmp(argv[i], "-s", 2) == 0) {
				isShowArgFound = true;
			}

			if (strncmp(argv[i], "-c", 2) == 0 && i + 1 < argc) {
				configName = argv[i + 1];
				configNameFound = true;
			}
		}
	}

	string configFileToRead;

	if (dynamicConfigPathFound) {
		configFileToRead = dynamicConfigPath;
	}
	else if (configNameFound) {
		configFileToRead = getConfigPath() + "\\" + configName;
	}

	if (isValidFilePath(configFileToRead)) {
		string command;
		ifstream iConfigFile(configFileToRead);

		if (!iConfigFile.is_open()) {
			cerr << "[!] could not open \"" << configFileToRead << "\"" << endl;
		}
		int i = 1;
		while (getline(iConfigFile, command)) {
			if (isShowArgFound) {
				cout << "[+] Command " << i << " => \"" << command << "\"" << "\n";
			}
			else {
				cout << "[+] Executing \"" << command << "\"" << "\n";
				createProcess((char*)(command.c_str()));
			}
			i++;
		}
	}
	else {
		cerr << "[!] Not a valid config file!\n";
		exit(1);
	}

	return 0;
}