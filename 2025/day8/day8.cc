#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <queue>
#include <vector>
#include <functional>

#include "../../file_utils/file_parser.cc"

namespace
{
	class Point
	{
		public:
			Point() = default;
			~Point() = default;
			uint64_t GetX() const { return x_; }
			uint64_t GetY() const { return y_; }
			uint64_t GetZ() const { return z_; }
			Point& SetX(uint64_t x)
			{
				x_ = x;
				return *this;
			}
			Point& SetY(uint64_t y)
			{
				y_ = y;
				return *this;
			}
			Point& SetZ(uint64_t z)
			{
				z_ = z;
				return *this;
			}
			static uint64_t SquaredDistance(Point a, Point b)
			{
			    int64_t dx = static_cast<int64_t>(a.GetX()) - static_cast<int64_t>(b.GetX());
			    int64_t dy = static_cast<int64_t>(a.GetY()) - static_cast<int64_t>(b.GetY());
			    int64_t dz = static_cast<int64_t>(a.GetZ()) - static_cast<int64_t>(b.GetZ());
			    return (dx * dx) + (dy * dy) + (dz * dz);
			}
			static Point Build(std::string line)
			{
				std::vector<uint64_t> coordinates;
				size_t l = 0;
				for (size_t r = 0; r < line.size(); ++r)
				{
					if (line[r] == ',')
					{
						coordinates.push_back(std::stoull(line.substr(l, r - l)));
						l = r + 1;
					}
				}
				if (l < line.size())
				{
					coordinates.push_back(std::stoull(line.substr(l, line.size() - l)));
				}
				return Point()
					.SetX(coordinates[0])
					.SetY(coordinates[1])
					.SetZ(coordinates[2]);
			}
			void Print()
			{
				std::cout << "x: " << x_ << " y: " << y_ << " z: " << z_ << std::endl;
			}
		private:
			uint64_t x_;
			uint64_t y_;
			uint64_t z_;
	};

	struct Edge
	{
		size_t pt1; // Index of first Point in edge.
		size_t pt2; // Inex of second Point in edge.
		uint64_t distance_squared;

		bool operator>(const Edge& other)
		{
			return distance_squared > other.distance_squared;
		}
		bool operator<(const Edge& other)
		{
			return distance_squared < other.distance_squared;
		}
	};

	class UnionFind
	{
		public:
			UnionFind(size_t size)
			{
				parents_.resize(size);
				ranks_.resize(size);
				for (size_t i = 0; i < size; ++i)
				{
					parents_[i] = i;
					ranks_[i] = 1;
				}
			}
			size_t Find(size_t p)
			{
				if (p == parents_[p]) return p;
				return parents_[p] = Find(parents_[p]);
			}
			void Union(size_t p1, size_t p2)
			{
				size_t par1 = Find(p1);
				size_t par2 = Find(p2);
				if (par1 == par2) return;

				if (ranks_[par1] < ranks_[par2])
				{
					parents_[par1] = par2;
					ranks_[par2] += ranks_[par1];
				}
				else if (ranks_[par1] > ranks_[par2])
				{
					parents_[par2] = par1;
					ranks_[par1] += ranks_[par2];
				}
				else
				{
					parents_[par1] = par2;
					ranks_[par2] += ranks_[par1];
				}
			}
			bool CompletelyConnected(size_t size)
			{
				return *std::max_element(ranks_.begin(), ranks_.end()) == size;
			}
			uint64_t ThreeLargest()
			{
				std::vector<size_t> roots;
				for (size_t i = 0; i < parents_.size(); ++i)
				{
					if (i == parents_[i])
					{
						roots.push_back(ranks_[i]);
					}
				}
				std::sort(roots.begin(), roots.end(), std::greater<size_t>());
				return roots[0] * roots[1] * roots[2];
			}


		private:
			std::vector<size_t> parents_;
			std::vector<size_t> ranks_;
	};
}  // namespace

int main()
{
	FileParser input_file("input8-2025.txt");
	std::vector<Point> points;
	while (input_file.HasNextLine())
	{
		points.push_back(Point::Build(input_file.GetLine()));
	}
	std::vector<Edge> edges;
	for (size_t i = 0; i < points.size(); ++i)
	{
		for (size_t j = i + 1; j < points.size(); ++j)
		{
			edges.push_back({ .pt1=i, .pt2=j, .distance_squared=Point::SquaredDistance(points[i], points[j]) });
		}
	}
	std::sort(edges.begin(), edges.end());

	UnionFind uf(points.size());
	for (int i = 0; i < edges.size(); ++i)
	{
		uf.Union(edges[i].pt1, edges[i].pt2);
		if (uf.CompletelyConnected(points.size()))
		{
			std::cout << "Result: " << points[edges[i].pt1].GetX() * points[edges[i].pt2].GetX() << std::endl;
			break;
		}
	}
}
