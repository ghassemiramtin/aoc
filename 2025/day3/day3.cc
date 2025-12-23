#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

#include <stdint.h>

#include "../../file_utils/file_parser.cc"

namespace
{
	// Finds the index of the maximum value in input
	// starting from index start.
	int FindMaxIndex(std::string_view input, int start_index, int end_index)
	{
		int prev_max = std::numeric_limits<int>::min();
		int max_index = start_index;
		for (; start_index < end_index; ++start_index)
		{
			if (input[start_index] - '0' > prev_max) {
				prev_max = input[start_index] - '0';
				max_index = start_index;
			}
		}
		return max_index;
	}

	// Finds the largest n-digit number from input_line.
	uint64_t FindVoltage(std::string_view input_line, int number_size)
	{
		uint64_t voltage = 0;
		int last_index = 0;
		for (int i = (number_size - 1); i >= 0; --i)
		{
			last_index = FindMaxIndex(input_line, last_index, input_line.size() - i);
			voltage = voltage * 10 + (input_line[last_index++] - '0');
		}
		std::cout << input_line << ": ";
		std::cout << voltage << std::endl;
		return voltage;
	}
}  // namespace


int main(int argc, char* argv[])
{
	FileParser input_file("input3-2025.txt");

	uint64_t total = 0;
	while (input_file.HasNextLine())
	{
		total += FindVoltage(input_file.GetLine(), 12);
	}
	std::cout << "Result is: " << total << std::endl;
}
