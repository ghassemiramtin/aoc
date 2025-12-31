#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <vector>

#include "../../file_utils/file_parser.cc"
#include "../../string_utils/string_parser.cc"

namespace
{
	std::vector<int> operator+(const std::vector<int>& a, const std::vector<int>& b)
	{
		std::vector<int> res;
		res.reserve(a.size());
		for (size_t i = 0; i < a.size(); ++i)
		{
			res.push_back(a[i] + b[i]);
		}
		return res;
	}

	class StateMachine
	{
		public:
			StateMachine(const std::string& line)
			{
				std::vector<std::string> tokens = utils::str::StrSplit(line, ' ');
				InitGoalState(tokens[0]);
				InitGoalVoltages(tokens[tokens.size() - 1]);
				for (size_t i = 1; i < tokens.size() - 1; ++i)
				{
					AddToggle(tokens[i]);
					AddSwitch(tokens[i]);
				}
			}
			int MinimumToggles()
			{
				std::queue<int> q;
				std::map<int, int> state_to_num_toggles;
				q.push(0);

				while (!q.empty())
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
			int MinimumVoltages()
			{
				std::queue<std::vector<int>> q;
				std::map<std::vector<int>, int> state_to_num_toggles;
				std::vector<int> initial_state(goal_voltages_.size(), 0);
				q.push(initial_state);

				while (!q.empty())
				{
					std::vector<int> current_state = q.front();
					q.pop();

					if (current_state == goal_voltages_) return state_to_num_toggles[current_state];
					int distance = state_to_num_toggles[current_state];

					for (const auto swch : switches_)
					{
						std::vector<int> next_state = current_state + swch;
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
			// Goal state for part A.
			int goal_state_ = 0;
			// Goal state for part B.
			std::vector<int> goal_voltages_;
			std::vector<int> toggles_;
			std::vector<std::vector<int>> switches_;
			void AddSwitch(const std::string& line)
			{
				std::vector<int> vec(goal_voltages_.size(), 0);
				std::vector<std::string> swch = utils::str::StrSplit(line.begin() + 1, line.end() - 1, ',');
				for (const auto& s : swch)
				{
					vec[std::stoi(s)]++;
				}
				switches_.push_back(vec);
			}
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
			// Parses the voltages and tracks the initial state.
			// Example voltage: {1,2,3,4}
			void InitGoalVoltages(const std::string& line)
			{
				std::vector<std::string> voltages = utils::str::StrSplit(line.begin() + 1, line.end() - 1, ',');
				for (const auto& voltage : voltages)
				{
					goal_voltages_.push_back(std::stoi(voltage));
				}
			}
	};
}  // namespace

int main()
{
	FileParser input_file("./input10-2025.txt");
	uint64_t result_a = 0;
	uint64_t result_b = 0;
	while (input_file.HasNextLine())
	{
		StateMachine state(input_file.GetLine());
		result_a += state.MinimumToggles();
		result_b += state.MinimumVoltages();
	}
	std::cout << "Result A: " << result_a << std::endl;
	std::cout << "Result B: " << result_b << std::endl;
}
