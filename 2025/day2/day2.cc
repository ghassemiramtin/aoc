#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stddef.h>

#include "../../file_utils/file_parser.cc"

namespace {
	// Checks whether the given ID repeats itself.
	// For example 123123 is invalid since 123 repeats twice.
	bool IsIdInvalid(std::string id) {
		// If the ID has an odd number of digits it can't repeat twice.
		if (id.size() % 2 == 1) {
			return false;
		}

		int front = 0;
		int back = id.size() - 1;

		while (front < back) {
			if (id[front] != id[back]) {
				return false;
			}
			front++;
			back--;
		}
		return true;
	}

	std::vector<std::string> StrSplit(std::string_view line, char delimiter) {
		std::vector<string> tokens;
		std::string token;

		std::istringstream token_stream(line);

		while(std::getline(token_stream, token, delimiter)) {
			tokens.push_back(token);
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

	while (input_file.HasNextLine()) {
		std::vector<std::string> range = StrSplit(input_file.GetLine(), '-');
		std::cout << range[0] << " " << range[1] << std::endl;
	}
}
