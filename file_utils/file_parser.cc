#include <fstream>
#include <iostream>
#include <string>

class FileParser {
	public:
		FileParser(std::string filename) : file_(filename) {}
		FileParser(std::string filename, char delimiter) : file_(filename), delimiter_(delimiter) {}
		~FileParser() = default;
		bool HasNextLine();
		std::string GetLine();
	private:
		std::ifstream file_;
		std::string line_;
		char delimiter_ = '\n';
};

bool FileParser::HasNextLine() {
	if (std::getline(file_, line_, delimiter_)) {
			return true;
	}
	return false;
}

std::string FileParser::GetLine() {
	return line_;
}

