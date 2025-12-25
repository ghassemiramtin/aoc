#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "../../file_utils/file_parser.cc"

using Range = std::pair<uint64_t, uint64_t>;

namespace
{
	Range CreateRange(std::string line)
	{
		std::vector<std::string> tokens;
		int left = 0;
		int right;
		for (right = 0; right < line.size(); ++right)
		{
			if (line[right] == '-')
			{
				tokens.push_back(line.substr(left, right - left));
				left = right + 1;
			}
		}
		if (left < line.size())
		{
			tokens.push_back(line.substr(left, right - left));
		}
		Range range(std::stoull(tokens[0]), std::stoull(tokens[1]));
		return range;
	}

	std::vector<Range> MergeRanges(std::vector<Range>& ranges)
	{
		std::sort(ranges.begin(), ranges.end());
		std::vector<Range> merged_ranges;
		Range current_range = ranges[0];

		for (int i = 1; i < ranges.size(); ++i)
		{
			if (current_range.second >= ranges[i].first)
			{
				current_range.second = std::max(current_range.second, ranges[i].second);
			}
			else
			{
				merged_ranges.push_back(current_range);
				current_range = ranges[i];
			}
		}
		merged_ranges.push_back(current_range);
		return merged_ranges;
	}

	bool ContainsId(const std::vector<Range>& ranges, uint64_t id)
	{
		for (const auto& range : ranges)
		{
			if (range.first <= id && id <= range.second) return true;
		}
		return false;
	}

	uint64_t NumFreshIds(uint64_t start, uint64_t end)
	{
		return end - start + 1;
	}
}  // namespace

bool flag = false;
int main()
{
	FileParser file_input("./input5-2025.txt");
	std::vector<Range> ranges;
	std::vector<uint64_t> ids;
	while (file_input.HasNextLine())
	{
		std::string line = file_input.GetLine();
		if (line.size() == 0) 
		{
			flag = true;
			continue;
		}
		if (!flag)
		{
			ranges.push_back(CreateRange(line));
		}
		else
		{
			ids.push_back(std::stoull(line));
		}
	}
	// Call merge intervals
	ranges = MergeRanges(ranges);

	uint64_t num_fresh = 0;
	uint64_t num_ids = 0;
	for (const auto& id : ids)
	{
		if (ContainsId(ranges, id)) ++num_fresh;
	}
	for (const auto& range : ranges)
	{
		num_ids += NumFreshIds(range.first, range.second);
	}

	std::cout << "Fresh: " << num_fresh << std::endl;
	std::cout << "Number of Fresh IDS: " << num_ids << std::endl;
}
