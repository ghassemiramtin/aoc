#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <vector>

#include "../../file_utils/file_parser.cc"

namespace
{
	class Accumulator
	{ 
		public:
			Accumulator() = default;
			~Accumulator() = default;
			
			uint64_t Accumulate(char op)
			{
				switch (op)
				{
					case '+':
						return Add();
					case '*':
						return Multiply();
					default:
						return 0;
				}
			}
			
			void Insert(std::string value)
			{
				values_.push_back(value);
			}

			void Clear()
			{
				values_.clear();
			}

			bool HasValues()
			{
				return !values_.empty();
			}
		private:
			std::vector<std::string> values_;
			uint64_t Add()
			{
				uint64_t total = 0;
				for (int i = 0; i < values_.size(); ++i)
				{
					total += std::stoull(values_[i]);
				}
				return total;
			}

			uint64_t Multiply()
			{
				uint64_t total = 1;
				for (int i = 0; i < values_.size(); ++i)
				{
					total *= std::stoull(values_[i]);
				}
				return total;
			}
	};
	bool IsOperator(char potential_op)
	{
		return potential_op == '*' || potential_op == '+';
	}
}  // namespace

int main()
{
	FileParser file_input("./input6-2025.txt");
	std::vector<std::string> grid;

	while (file_input.HasNextLine())
	{
		grid.push_back(file_input.GetLine());
	}

	uint64_t total = 0;
	Accumulator acc;
	char op;
	for (int c = grid[0].size() - 1; c >= 0; --c)
	{
		std::string column = "";
		for (int r = 0; r < grid.size(); ++r)
		{
			if (std::isspace(grid[r][c])) continue;
			if (IsOperator(grid[r][c]))
			{
				op = grid[r][c];
				continue;
			}
			column += grid[r][c];
		}
		// This means a column of all spaces, problem i is complete.
		if (column.empty())
		{
			total += acc.Accumulate(op);
			acc.Clear();
		}
		else
		{
			acc.Insert(column);
		}
	}
	if (acc.HasValues())
	{
		total += acc.Accumulate(op);
	}

	std::cout << "Total is: " << total << std::endl;
}
