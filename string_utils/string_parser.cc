#include <string>
#include <vector>

namespace utils::str
{
std::vector<std::string> StrSplit(const std::string& line, char delimiter)
{
	std::vector<std::string> tokens;
	size_t l = 0;
	for (size_t r = 0; r < line.size(); ++r)
	{
		if (line[r] == delimiter)
		{
			tokens.push_back(line.substr(l, r - l));
			l = r + 1;
		}
	}
	if (l < line.size()) tokens.push_back(line.substr(l, line.size() - l));
	return tokens;
}

template <typename Iterator>
std::vector<std::string> StrSplit(Iterator begin, Iterator end, char delimiter)
{
	std::string line(begin, end);
	return StrSplit(line, delimiter);
}
}  //  utils::str
