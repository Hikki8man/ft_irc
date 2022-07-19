#include "../includes/common.hpp"

std::vector<std::string> split(const std::string& s, const std::string& delim) {
	std::string str(s);
	std::vector<std::string> result;

	while (!str.empty()) {
		size_t pos = str.find(delim);
		if (pos == std::string::npos) {
			result.push_back(str);
			break;
		}
		result.push_back(str.substr(0, pos));
		str = str.substr(pos + 1);
	}
	return result;
}