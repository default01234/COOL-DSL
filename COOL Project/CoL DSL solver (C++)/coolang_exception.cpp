#include "coolang_exception.hpp"
namespace COOLANG {
	CalculationException::CalculationException(CalculationErrorType type, const std::string& msg,
		const std::string& file, int line, const std::string& func)
		: errorType(type), message(msg), file(file), line(line), func(func) {
		// Optionally, append file, line, and function info to the message
		std::ostringstream oss;
		oss << message << " (File: " << file << ", Line: " << line << ", Function: " << func << ")";
		message = oss.str();

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = bbhline + message + bbhline; // Replace with actual info
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif

	}
	ReasoningException::ReasoningException(ReasoningErrorType type, const std::string& msg,
		const std::string& file, int line, const std::string& func)
		: errorType(type), message(msg), file(file), line(line), func(func) {
		// Optionally, append file, line, and function info to the message
		std::ostringstream oss;
		oss << message << " (File: " << file << ", Line: " << line << ", Function: " << func << ")";
		message = oss.str();

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = bbhline + message + bbhline; // Replace with actual info
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif

	}








}