#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include "../../file_utils/file_parser.cc"

using Coordinates = std::pair<int, int>;
namespace
{
	bool IsValidIndex(int row, int col, const std::vector<std::vector<char>>& map)
	{
		return (row >= 0 && row < map.size() && col >= 0 && col < map[0].size());
	}
	int Count(int row, int col, const std::vector<std::vector<char>>& map)
	{
		if (!IsValidIndex(row, col, map)) return 0;
		return map[row][col] == '.' ? 0 : 1;

	}
	int CountAdjacent(int row, int col, std::vector<std::vector<char>> &map)
	{
		if (map[row][col] == '.') return 0;
		if (Count(row + 1, col + 1, map)
			+ Count(row + 1, col, map)
			+ Count(row + 1, col - 1, map)
			+ Count(row, col + 1, map)
			+ Count(row, col - 1, map)
			+ Count(row - 1, col - 1, map)
			+ Count(row - 1, col + 1, map)
			+ Count(row - 1, col, map) < 4) return 1;
		return 0;
	}
}  // namespace

int main()
{
	FileParser input_file("input4-2025.txt");

	// Map a position X, Y to the number of paper towels
	// in every adjacent direction
	std::vector<std::vector<char>> map;
	std::string line;
	while (input_file.HasNextLine())
	{
		line = input_file.GetLine();
		std::vector<char> single_row(line.begin(), line.end());
		map.push_back(single_row);
	}
	// Do a first pass to count adjacent cells
	int total = 0;
	for (int r = 0; r < map.size(); ++r)
	{
		for (int c = 0; c < map[0].size(); ++c)
		{
			total += CountAdjacent(r, c, map);
		}
	}
	std::cout << total << std::endl;
}
