#include"pipeCommunication.hpp"
int launchClientSide(const std::string& clientcmd, const bool& detach /*= true*/,
	const bool& show_window /*= false*/, const bool& block /*= false*/) {
	char cmd_[256];
	strncpy_s(cmd_, clientcmd.c_str(), sizeof(cmd_) - 1);
	cmd_[sizeof(cmd_) - 1] = '\0';

	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, cmd_, -1, nullptr, 0);
	LPWSTR cmdWide = new WCHAR[requiredSize];
	MultiByteToWideChar(CP_UTF8, 0, cmd_, -1, cmdWide, requiredSize);


	DWORD flags = 0;
	if (detach && !show_window) {
		flags = DETACHED_PROCESS;
	}
	else if (!detach && show_window) {
		flags = CREATE_NEW_CONSOLE;
	}
	else if (detach && show_window) {
#if debug
		{
			static int ignorecount = 0;
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0" << ")\tlaunchClientSide(const std::string&, const bool&=true, const bool&=false, const bool&=false)" << " Invalid arguments, throw(-1) "
				<< "\tignorecount:["
				<< ignorecount++ << "\t]" << "\n"/*"\n"*/;
		}
#endif
		throw(-1);

	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.

	if (!CreateProcess(
		nullptr,         // No module name (use command line)
		cmdWide,         // Command line
		nullptr,         // Process handle not inheritable
		nullptr,         // Thread handle not inheritable
		FALSE,         // Set handle inheritance to FALSE
		flags, // Detaches the process
		nullptr,         // Use parent's environment block
		nullptr,         // Use parent's starting directory
		&si,         // Pointer to STARTUPINFO structure
		&pi         // Pointer to PROCESS_INFORMATION structure
	)) {
		//        printf("CreateProcess failed (%d)\n", GetLastError());
#if debug
		{
			static int ignorecount = 0;
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0" << ")\tlaunchClientSide(const std::string&, const bool&=true, const bool&=false, const bool&=false)" << " , CreateProcess failed, throw(-1) "
				<< "\tignorecount:["
				<< ignorecount++ << "\t]" << "\n"/*"\n"*/;
		}
#endif
		throw(-1);
	}

	// Do not wait until child process exits (this is non-blocking)
	if (block) {
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	// Close process and thread handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// Continue with other tasks
	delete[] cmdWide;

	return 0;
}


bool isPotentialJsonString(const std::string& s) {
	std::stack<char> brackets;

	for (char ch : s) {
		if (ch == '{' || ch == '[') {
			brackets.push(ch);
		}
		else if (ch == '}') {
			if (brackets.empty() || brackets.top() != '{') {
				return false;
			}
			brackets.pop();
		}
		else if (ch == ']') {
			if (brackets.empty() || brackets.top() != '[') {
				return false;
			}
			brackets.pop();
		}
	}

	return brackets.empty();
}

std::string generateFullPath(const std::string& directory,
	const std::string& name) {
	std::string fullPath = directory;

	// If the directory doesn't end with a path separator, add one.
	if (!fullPath.empty() && fullPath.back() != '\\'
		&& fullPath.back() != '/') {
		fullPath += '\\'; // Using backslash as default path separator for Windows.
	}

	fullPath += name;

	return fullPath;
}
HANDLE connectOrCreatePipe(const std::string& pipeName) {
	auto original_name = pipeName;
	std::replace(original_name.begin(),original_name.end(), '\\', '_');
	std::replace(original_name.begin(),original_name.end(), '/', '_');
	std::replace(original_name.begin(),original_name.end(), ':', '_');
	
	std::string fullPipeName = "\\\\.\\pipe\\" + original_name;
	HANDLE hPipe;

	// First, try to connect to the pipe
	hPipe = CreateFileA(fullPipeName.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0,
		nullptr,
		OPEN_EXISTING, 0,
		nullptr);

	// Check if connection was successful
	if (hPipe == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			// Pipe does not exist, create it and wait for a client
			hPipe = CreateNamedPipeA(fullPipeName.c_str(),
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES, 65536,  // Output buffer size
				65536,  // Input buffer size
				NMPWAIT_USE_DEFAULT_WAIT,
				nullptr);

			if (hPipe == INVALID_HANDLE_VALUE) {
				std::cerr << "Failed to create named pipe. Error: "
					<< GetLastError() << "\n";
				return INVALID_HANDLE_VALUE;
			}

			// Wait for a client to connect
			BOOL connected = ConnectNamedPipe(hPipe, nullptr);
			if (!connected && GetLastError() != ERROR_PIPE_CONNECTED) {
				std::cerr << "Failed to connect to named pipe. Error: "
					<< GetLastError() << "\n";
				CloseHandle(hPipe);
				return INVALID_HANDLE_VALUE;
			}
		}
		else {
			std::cerr << "Failed to open named pipe. Error: " << GetLastError()
				<< "\n";
			return INVALID_HANDLE_VALUE;
		}
	}

	return hPipe;
}

bool setPipeReadMode(HANDLE hPipe) {
	// Set pipe mode to read and ensure others can write
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL success = SetNamedPipeHandleState(hPipe,    // pipe handle
		&dwMode,  // new pipe mode
		nullptr,     // don't set maximum bytes
		nullptr      // don't set maximum time
	);

	if (!success) {
		std::cerr << "Failed to set pipe to read mode. Error: "
			<< GetLastError() << "\n";
		return false;
	}

	return true;
}
bool setPipeWriteMode(HANDLE hPipe) {
	// Set pipe mode to write and ensure others can read
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL success = SetNamedPipeHandleState(hPipe,    // pipe handle
		&dwMode,  // new pipe mode
		nullptr,     // don't set maximum bytes
		nullptr      // don't set maximum time
	);

	if (!success) {
		std::cerr << "Failed to set pipe to write mode. Error: "
			<< GetLastError() << "\n";
		return false;
	}

	return true;
}
std::string readJsonStringFromPipe(HANDLE hPipe,
	bool (*isJsonString)(const std::string&)) {
	const DWORD bufferSize = 65536;
	char buffer[bufferSize];
	BOOL success = FALSE;
	DWORD bytesRead = 0;

	std::string accumulatedData;

	do {
		success = ReadFile(hPipe,                 // pipe handle
			buffer,                // buffer to receive reply
			bufferSize,            // size of buffer
			&bytesRead,            // number of bytes read
			nullptr                   // not overlapped
		);

		if (!success && GetLastError() != ERROR_MORE_DATA) {
			std::cerr << "ReadFile failed. Error: " << GetLastError()
				<< "\n";
			break;
		}

		accumulatedData += std::string(buffer, bytesRead);

	} while (!isJsonString(accumulatedData)); // Keep reading until we have a valid JSON string

	return accumulatedData;
}
bool writeStringToPipe(HANDLE hPipe, const std::string& message) {
	DWORD bytesWritten = 0;
	BOOL success = WriteFile(hPipe,                   // pipe handle
		message.c_str(),         // message
		message.size(),          // message length
		&bytesWritten,           // bytes written
		nullptr                     // not overlapped
	);

	if (!success || bytesWritten == 0) {
		std::cerr << "WriteFile to pipe failed. Error: " << GetLastError()
			<< "\n";
		return false;
	}

	if (bytesWritten != message.size()) {
		std::cerr << "Not all bytes were written to pipe." << "\n";
		return false;
	}

	return true;
}

void closePipe(HANDLE hPipe) {
	if (hPipe != INVALID_HANDLE_VALUE) {
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
	}
}

/******************************************

PipeCommunicationHandler

***********************************/
// Constructor
PipeCommunicationHandler::PipeCommunicationHandler(const std::string& directoryPath,
	const std::string& fileNameStem, const std::string& mode) :
	openMode(mode) {

	if (mode == "read") {
		hPipe = connectOrCreatePipe(
			generateFullPath(directoryPath, fileNameStem));
		setPipeReadMode(hPipe);
	}
	else if (mode == "write") {
		hPipe = connectOrCreatePipe(
			generateFullPath(directoryPath, fileNameStem));
		setPipeWriteMode(hPipe);
	}
	else {
		std::cerr << "Invalid mode. Please choose 'read' or 'write'."
			<< "\n";
	}
}
std::string PipeCommunicationHandler::readJsonMessage() {
	if (openMode != "read") {
		std::cerr << "Error: Pipe is not in read mode." << "\n";
		return "";
	}
	return readJsonStringFromPipe(hPipe, isPotentialJsonString);
}
bool PipeCommunicationHandler::writeMessage(const std::string& message) {
	if (openMode != "write") {
		std::cerr << "Error: Pipe is not in write mode." << "\n";
		return false;
	}
	return writeStringToPipe(hPipe, message);
}
void PipeCommunicationHandler::closeConnection() {
	closePipe(hPipe);
}
PipeCommunicationHandler::~PipeCommunicationHandler() {
	closePipe(hPipe);
}