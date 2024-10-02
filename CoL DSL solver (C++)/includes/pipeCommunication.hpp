#ifndef PIPECOMMUNICATION_HPP
#define PIPECOMMUNICATION_HPP

#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <filesystem>
#include <stack>

using namespace std;

int launchClientSide(const std::string& clientcmd, const bool& detach = true,
	const bool& show_window = false, const bool& block = false);

bool isPotentialJsonString(const std::string& s);

std::string generateFullPath(const std::string& directory,
	const std::string& name);


HANDLE connectOrCreatePipe(const std::string& pipeName);

bool setPipeReadMode(HANDLE hPipe);


bool setPipeWriteMode(HANDLE hPipe);


std::string readJsonStringFromPipe(HANDLE hPipe,
	bool (*isJsonString)(const std::string&));



bool writeStringToPipe(HANDLE hPipe, const std::string& message);


void closePipe(HANDLE hPipe);


class PipeCommunicationHandler {
private:
	HANDLE hPipe;
	std::string openMode;

public:
	// Constructor
	PipeCommunicationHandler(const std::string& directoryPath,
		const std::string& fileNameStem, const std::string& mode);


	// Member Function 1
	std::string readJsonMessage();

	// Member Function 2
	bool writeMessage(const std::string& message);

	// Member Function 3
	void closeConnection();

	// Destructor
	~PipeCommunicationHandler();

};
//int main() {
//    std::string &&coolang_home = getenv("COOLANG_HOME");
//
//    // Sample usage for read
//    PipeCommunicationHandler reader(coolang_home, "mystem1", "read");
//    // Sample usage for write
//    PipeCommunicationHandler writer(coolang_home, "mystem2", "write");
//
//    int i = 0;
//    while (true) {
//        std::cout << reader.readJsonMessage();
//        std::stringstream ss;
//        ss<<std::string("{\"key\":") << i<<"}";
//
//
//        writer.writeMessage(ss.str());
//        i++;
//
//    }
//
//    return 0;
//}

#endif

