#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <queue>
#include <set>

#include "../../file_utils/file_parser.cc"

using Coordinates = std::pair<int, int>;
namespace
{
	bool IsValidIndex(int row, int col, const std::vector<std::vector<int>>& map)
	{
		return (row >= 0 && row < map.size() && col >= 0 && col < map[0].size());
	}
	int GetIndex(int row, int col, const std::vector<std::vector<int>>& map)
	{
		if (!IsValidIndex(row, col, map)) return 0;
		return map[row][col] == 0 ? 0 : 1;
	}
	int CountAdjacent(int row, int col, std::vector<std::vector<int>> &map)
	{
		return GetIndex(row + 1, col + 1, map)
			+ GetIndex(row + 1, col, map)
			+ GetIndex(row + 1, col - 1, map)
			+ GetIndex(row, col + 1, map)
			+ GetIndex(row, col - 1, map)
			+ GetIndex(row - 1, col - 1, map)
			+ GetIndex(row - 1, col + 1, map)
			+ GetIndex(row - 1, col, map);
	}
}  // namespace

int main()
{
	FileParser input_file("input4-2025.txt");

	// Map a position X, Y to the number of paper towels
	// in every adjacent direction
	std::vector<std::vector<int>> map;
	while (input_file.HasNextLine())
	{
		std::vector<int> single_row;
		for(const auto& c : input_file.GetLine())
		{
			single_row.push_back(c == '@' ? 1 : 0);
		}
		map.push_back(single_row);
	}
	std::queue<Coordinates> queue;
	std::set<Coordinates> visited;
	// Do a first pass to count adjacent cells
	for (int r = 0; r < map.size(); ++r)
	{
		for (int c = 0; c < map[0].size(); ++c)
		{
			if (map[r][c] == 0) continue;
			map[r][c] = CountAdjacent(r, c, map);
			if (map[r][c] < 4)
			{
				queue.push({r, c});
				visited.insert({r, c});
			}
		}
	}

	int total = 0;
	std::vector<Coordinates> deltas = {
		{1, 1},
		{1, 0},
		{1, -1},
		{0, 1},
		{0, -1},
		{-1, 1},
		{-1, 0},
		{-1, -1}
	};
	while (!queue.empty())
	{
		Coordinates coordinate = queue.front();
		queue.pop();
		for (const auto& delta : deltas)
		{
			int dr = delta.first + coordinate.first;
			int dc = delta.second + coordinate.second;
			if (IsValidIndex(dr, dc, map) && visited.find({dr, dc}) == visited.end() && map[dr][dc] > 0)
			{
				map[dr][dc]--;
				if (map[dr][dc] < 4)
				{
					queue.push({dr, dc});
					visited.insert({dr, dc});
				}
			}
		}
		map[coordinate.first][coordinate.second] = 0;
		++total;
	}

	std::cout << "Result is: " << total << std::endl;

}
