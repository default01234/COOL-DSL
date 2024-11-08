#include "coolang_utility.hpp"
/**
 * @brief output formatted date "yy(yyyy)mmddhhmmss" or other formats you want.
 * @param format
 * @return
 */
std::string getFormattedDate(const std::string& format) {
	// Get the current time
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);

	std::stringstream ss;

	for (size_t i = 0; i < format.size(); ++i) {
		switch (format[i]) {
		case 'y':
			if (i + 3 < format.size() && format[i + 1] == 'y'
				&& format[i + 2] == 'y' && format[i + 3] == 'y') {
				ss << std::setfill('0') << std::setw(4)
					<< localTime->tm_year + 1900;
				i += 3;
			}
			else if (i + 1 < format.size() && format[i + 1] == 'y') {
				ss << std::setfill('0') << std::setw(2)
					<< (localTime->tm_year + 1900) % 100;
				i++;
			}
			break;
		case 'm':
			ss << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1);
			i += 1; // Skip next 'm'
			break;
		case 'd':
			ss << std::setfill('0') << std::setw(2) << localTime->tm_mday;
			i += 1; // Skip next 'd'
			break;
		case 'h':
			ss << std::setfill('0') << std::setw(2) << localTime->tm_hour;
			i += 1; // Skip next 'h'
			break;
		case 's':
			ss << std::setfill('0') << std::setw(2) << localTime->tm_sec;
			i += 1; // Skip next 's'
			break;
		default:
			// Do nothing for other characters
			break;
		}
	}

	return ss.str();
}
