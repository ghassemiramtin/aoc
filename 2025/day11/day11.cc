#include <iostream>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <tuple>

#include "../../file_utils/file_parser.cc"
#include "../../string_utils/string_parser.cc"


struct TupleHash {
    std::size_t operator()(
        const std::tuple<std::string, bool, bool>& t
    ) const {
        const auto& [s, b1, b2] = t;
        std::size_t h1 = std::hash<std::string>{}(s);
        std::size_t h2 = std::hash<bool>{}(b1);
        std::size_t h3 = std::hash<bool>{}(b2);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
using State = std::tuple<std::string, bool, bool>;
std::unordered_map<State, uint64_t, TupleHash> cache;

uint64_t Dfs(std::string curr,
		const std::unordered_map<std::string,
		std::vector<std::string>>& server_rack,
		bool has_dac, bool has_fft)
{
	if (curr == "out") return has_dac && has_fft ? 1 : 0;
	if (curr == "dac") has_dac = true;
	if (curr == "fft") has_fft = true;
	auto it = server_rack.find(curr);
	if (it == server_rack.end()) return 0;
	State state{curr, has_dac, has_fft};
    	if (auto it = cache.find(state); it != cache.end())
        	return it->second;
	uint64_t res = 0;
	for (const auto& next : it->second)
	{
		res += Dfs(next, server_rack, has_dac, has_fft);
	}
	return cache[state] = res;
}

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
	std::cout << "Recursive Dfs: " << Dfs("svr", server_rack, false, false) << std::endl;
}
