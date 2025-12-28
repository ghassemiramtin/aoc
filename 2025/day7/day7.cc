#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../../file_utils/file_parser.cc"

namespace
{
void ResolvePath(int col, uint64_t count, const std::string& current_row, std::map<int, uint64_t>& next_row_counts)
{
	if (col < 0 || col >= current_row.length()) return;

    	if (current_row[col] == '^') {
        	ResolvePath(col - 1, count, current_row, next_row_counts);
        	ResolvePath(col + 1, count, current_row, next_row_counts);
    	} else {
        	next_row_counts[col] += count;
    	}
}
}  // namespace

int main()
{
	std::vector<std::string> manifold;
    	FileParser f("./input7-2025.txt");

    	while (f.HasNextLine())
	{
        	manifold.push_back(f.GetLine());
    	}

	int start_col = manifold[0].find('S');
    
	std::map<int, uint64_t> current_counts;
    	current_counts[start_col] = 1;

    	for (size_t r = 1; r < manifold.size(); ++r)
	{
		std::map<int, uint64_t> next_counts;
        
        	for (auto const& [col, count] : current_counts)
		{
            		ResolvePath(col, count, manifold[r], next_counts);
        	}
        
        	current_counts = next_counts;
    	}

    	uint64_t total_timelines = 0;
    	for (auto const& [col, count] : current_counts)
	{
        	total_timelines += count;
    	}

	std::cout << "Total unique timelines: " << total_timelines << std::endl;

    	return 0;
}
