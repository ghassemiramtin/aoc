#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <vector>

#include "../../file_utils/file_parser.cc"
#include "../../string_utils/string_parser.cc"

namespace
{
	class StateMachine
	{
		public:
			StateMachine(const std::string& line)
			{
				std::vector<std::string> tokens = utils::str::StrSplit(line, ' ');
				InitGoalState(tokens[0]);
				for (size_t i = 1; i < tokens.size() - 1; ++i)
				{
					AddToggle(tokens[i]);
				}
			}
			int MinimumToggles()
			{
				std::queue<int> q;
				std::map<int, int> state_to_num_toggles;
				q.push(0);

				while(!q.empty())
				{
					int current_state = q.front();
					q.pop();
					
					if (current_state == goal_state_) return state_to_num_toggles[current_state];
					int distance = state_to_num_toggles[current_state];

					for (const auto toggle : toggles_)
					{
						int next_state = current_state ^ toggle;
						if (state_to_num_toggles.find(next_state) == state_to_num_toggles.end())
						{
							state_to_num_toggles[next_state] = distance + 1;
							q.push(next_state);
						}
					}
				}
				return -1;
			}
		private:
			int goal_state_ = 0;
			std::vector<int> toggles_;
			// Take a toggle like (0, 1) and add it to toggles_
			void AddToggle(const std::string& line)
			{
				std::vector<std::string> shifts = utils::str::StrSplit(line.begin() + 1, line.end() - 1, ',');
				int toggle = 0;
				for (const auto& shift : shifts)
				{
					toggle |= (1 << std::stoi(shift));
				}
				toggles_.push_back(toggle);
			}
			// The initial state will be either the lights or voltages depending on the part.
			// [..#.]. Parse this by skipping first and last bracket.
			// {1,2,3,4} same here.
			void InitGoalState(const std::string& line)
			{
				for (size_t i = 1; i < line.size() - 1; ++i)
				{
					switch (line[i])
					{
						case '.':
							break;
						case '#':
							goal_state_ |= (1 << (i - 1));
							break;
					}
				}
			}
	};
}  // namespace

int main()
{
	FileParser input_file("./test.txt");
	uint64_t result = 0;
	while (input_file.HasNextLine())
	{
		StateMachine state(input_file.GetLine());
		result += state.MinimumToggles();
	}
	std::cout << "Result: " << result << std::endl;
}
