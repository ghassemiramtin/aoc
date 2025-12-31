#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../../file_utils/file_parser.cc"

namespace
{
	class Tile
	{
		public:
			Tile(std::string line)
			{
				size_t l = 0;
				std::vector<std::string> tokens;
				for (size_t r = 0; r < line.size(); ++r)
				{
					if (line[r] == ',')
					{
						tokens.push_back(line.substr(l, r - l));
						l = r + 1;
					}
				}
				if (l < line.size())
				{
					tokens.push_back(line.substr(l, line.size() - l));
				}
				x_ = std::stoi(tokens[0]);
				y_ = std::stoi(tokens[1]);
			}
			~Tile() = default;

			static uint64_t Area(const Tile& a, const Tile& b)
			{
				return ((a.x_ > b.x_ ? a.x_ - b.x_ : b.x_ - a.x_) + 1) * 
       					((a.y_ > b.y_ ? a.y_ - b.y_ : b.y_ - a.y_) + 1);
			}
			static uint64_t MaxArea(const std::vector<uint64_t>& areas)
			{
				uint64_t maximum = 0;
				for (const auto& area : areas)
				{
					if (area > maximum)
					{
						maximum = area;
					}
				}
				return maximum;
			}
			void Print()
			{
				std::cout << "X: " << x_ << " Y: " << y_ << std::endl;
			}
		private:
			size_t x_;
			size_t y_;
	};
}  // namespace

int main()
{
	FileParser input_file("./input9-2025.txt");
	std::vector<Tile> tiles;
	while (input_file.HasNextLine())
	{
		tiles.push_back(Tile(input_file.GetLine()));
	}

	std::vector<uint64_t> areas;
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		for (size_t j = i + 1; j < tiles.size(); ++j)
		{
			areas.push_back(Tile::Area(tiles[i], tiles[j]));
		}
	}
	std::cout << Tile::MaxArea(areas) << std::endl;
}

