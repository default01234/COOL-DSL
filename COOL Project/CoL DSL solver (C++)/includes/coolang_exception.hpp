#include <stdexcept>
#include <sstream>
#include <string>
#include "coolang_define.hpp"

#ifndef COOLANG_EXCEPTION_HPP_
#define COOLANG_EXCEPTION_HPP_

namespace COOLANG {
	enum class CalculationErrorType {
		None,
		Type,
		DivideByZero,
		Overflow,
		Underflow
	};

	class CalculationException : public std::exception {
	private:
		std::string message;
		CalculationErrorType errorType;
		std::string file;
		int line;
		std::string func;

	public:
		CalculationException(CalculationErrorType type, const std::string& msg,
			const std::string& file, int line, const std::string& func);

		inline const char* what() const noexcept override {
			return message.c_str();
		}

		inline CalculationErrorType getType() const {
			return errorType;
		}

		// Optionally, methods to retrieve file, line, and function
		inline std::string getFile() const {
			return file;
		}
		inline int getLine() const {
			return line;
		}
		inline std::string getFunction() const {
			return func;
		}
	};
#define THROW_CALCULATION_EXCEPTION(type, message) \
    throw CalculationException(type, message, __FILE__, __LINE__, __func__)




enum class ReasoningErrorType {
	RunOutOfResources,
	NoMoreSpacesToExplore
};

class ReasoningException : public std::exception {
private:
	std::string message;
	ReasoningErrorType errorType;
	std::string file;
	int line;
	std::string func;

public:
	ReasoningException(ReasoningErrorType type, const std::string& msg,
		const std::string& file, int line, const std::string& func);

	inline const char* what() const noexcept override {
		return message.c_str();
	}

	inline ReasoningErrorType getType() const {
		return errorType;
	}

	// Optionally, methods to retrieve file, line, and function
	inline std::string getFile() const {
		return file;
	}
	inline int getLine() const {
		return line;
	}
	inline std::string getFunction() const {
		return func;
	}
};
#define THROW_REASONING_EXCEPTION(type, message) \
    throw ReasoningException(type, message, __FILE__, __LINE__, __func__)

}

#endif