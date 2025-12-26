#include <iostream>
#include <sstream>
#include <vector>

#include "../../file_utils/file_parser.cc"

namespace
{
	uint64_t Add(const std::vector<std::string>& input)
	{
		uint64_t total = 0;
		for (int i = 0; i < input.size() - 1; ++i)
		{
			total += std::stoull(input[i]);
		}
		return total;
	}
	
	uint64_t Multiply(const std::vector<std::string>& input)
	{
		uint64_t total = 1;
		for (int i = 0; i < input.size() - 1; ++i)
		{
			total *= std::stoull(input[i]);
		}
		return total;
	}

	uint64_t Reduce(const std::vector<std::string>& input)
	{
		char operation = input.back()[0];
		switch (operation)
		{
			case '+':
				return Add(input);
			case '*':
				return Multiply(input);
			default:
				return 0;
		}
	}
}  // namespace

int main()
{
	FileParser file_input("./input6-2025.txt");
	std::vector<std::vector<std::string>> columns;

	while (file_input.HasNextLine())
	{
		std::string line = file_input.GetLine();
		std::stringstream ss(line);
		std::string token;
		int index = 0;

		while (ss >> token)
		{
			if (index >= columns.size()) columns.push_back({});
			columns[index].push_back(token);
			index++;
		}
	}
	uint64_t total = 0;
	for (const auto& col : columns)
	{
		total += Reduce(col);
	}
	std::cout << "Total: " << total << std::endl;
}
