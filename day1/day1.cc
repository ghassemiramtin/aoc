#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

// Parses the line and returns direction and magnitude via parameters
void parse_line(std::string line, char* direction, int* magnitude) {
	*direction = line[0];
	int num = std::stoi(line.substr(1));
	*magnitude = *direction == 'L' ? -num : num;
}

int dist_to_zero(int number, char direction) {
	switch (direction) {
		case 'L':
			return number;
		case 'R':
			return 100 - number;
		default:
			return 0;
	}
}

int passes_zero(int prev_value, int magnitude, char direction) {
	int distance = dist_to_zero(prev_value, direction);
	if (distance == 0) {
		return magnitude / 100;
	}
	if (magnitude < distance) {
		return 0;
	}
	magnitude -= distance;

	return 1 + magnitude / 100;
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
        	std::cerr << "Usage: " << argv[0] << " <input_file> <a|b>\n";
        	return 1;
    	}

	std::string filename = argv[1];
	std::string part = argv[2];

	std::ifstream input_file(filename);

	if (!input_file) {
		std::cerr << "Could not open " << filename << std::endl;
		return 1;
	}

	std::string line;
	int running_total = 50;
	int result = 0;
	char direction;
	int magnitude;
	while (std::getline(input_file, line)) {
		parse_line(line, &direction, &magnitude);
		if (part == "b") {
			result += passes_zero(running_total, std::abs(magnitude), direction); 
		}
		running_total = ((running_total + magnitude) % 100);
		if (running_total < 0) {
			running_total += 100;
		}
		if (part == "a" && running_total == 0) ++result;
	}
	std::cout << "The result is: " << result << std::endl;
	return 0;
}
