#include <cstdint>
#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "../../file_utils/file_parser.cc"

namespace {

	// Splits line into tokens of size token_size and returns all unique occurences of the tokens.
	std::set<std::string> UniqueTokens(std::string id, int token_size) {
		std::set<std::string> tokens;

		for (int i = 0; i < id.size(); i += token_size) {
			tokens.insert(id.substr(i, token_size));
		}
		return tokens;
	}

	// Checks whether the given ID repeats itself two or more times.
	// For example, 123123123 repeats itself three times.
	bool IsIdInvalid(std::string id) {
		int index = 0;
		int half_way_point = id.size() / 2;

		for (int i = half_way_point; i > 0; --i) {
			// If you can't break the id up into an equal number of tokens
			// continue on.
			if (id.size() % i != 0) {
				continue;
			}
			std::set<std::string> tokens = UniqueTokens(id, i);
			if (tokens.size() == 1) {
				return true;
			}
		}
		return false;
	}

	std::vector<std::string> StrSplit(std::string line, char delimiter) {
		std::vector<std::string> tokens;

		int left = 0;
		int right;
		for (right = 0; right < line.size(); ++right) {
			if (line[right] == delimiter) { 
				tokens.push_back(line.substr(left, right - left));
				left = right + 1;
			} 
		}
		if (left <= line.size()) {
			tokens.push_back(line.substr(left, right - left));
		}
		return tokens;
	}
}  // namespace

int main(int argc, char* argv[])
{
	if (argc != 2) {
        	std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        	return 1;
    	}

	std::string filename = argv[1];
	FileParser input_file(filename, ',');

	int64_t result = 0;
	while (input_file.HasNextLine()) {
		std::vector<std::string> range = StrSplit(input_file.GetLine(), '-');
		int64_t left = std::stoll(range[0]);
		int64_t right = std::stoll(range[1]);


		for (; left <= right; ++left) {
			if (IsIdInvalid(std::to_string(left))) {
				result += left;
			}
		}
	}
	std::cout << "Result is: " << result << std::endl;
	return 0;
}
