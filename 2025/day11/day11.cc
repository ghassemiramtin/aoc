#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

#include "../../file_utils/file_parser.cc"
#include "../../string_utils/string_parser.cc"

int main()
{
	FileParser input_file("./input11-2025.txt");
	std::unordered_map<std::string, std::vector<std::string>> server_rack;
	while (input_file.HasNextLine())
	{
		std::string line = input_file.GetLine();
		std::string key = line.substr(0, 3);
		for (const auto& s : utils::str::StrSplit(line.begin() + 5, line.end(), ' '))
		{
			server_rack[key].push_back(s);
		}
	}
	std::queue<std::string> q;
	q.push("you");
	uint64_t res = 0;
	while (!q.empty())
	{
		std::string curr = q.front();
		q.pop();

		if (curr == "out")
		{
			res++;
			continue;
		}

		for (const auto& next : server_rack[curr])
		{
			q.push(next);
		}
	}
	std::cout << "Result: " << res << std::endl;
}
