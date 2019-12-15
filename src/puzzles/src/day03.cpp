#include <catch/catch.hpp>

#include <iostream>
#include <functional>
#include <fstream>
#include <set>
#include <cassert>

#include <boost/algorithm/string.hpp>

class Point {
public:
  Point(): x_(0), y_(0) {}

  Point(const Point& other) {
    x_ = other.x_;
    y_ = other.y_;
  }

  Point(size_t x, size_t y): x_(x), y_(y) {}

  void moveRight() {
    x_++;
  }

  void moveLeft() {
    x_--;
  }

  void moveUp() {
    y_++;
  }

  void moveDown() {
    y_--;
  }

  bool operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

  bool operator<(const Point& other) const {
    return x_ < other.x_ || (x_ == other.x_ && y_ < other.y_);
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "{x=" << p.x_ << ",y=" << p.y_ << "}\n";
  }

  int getX() const { return x_; }

  int getY() const { return y_; }


private:
  int x_, y_;
};

using Points = std::set<Point>;
using Path= std::vector<Point>;

std::function<void(Point&)> GetFunction(char c) {
  std::function<void(Point&)> func;
  switch(c) {
    case 'R':
      func = [](Point& p){ p.moveRight(); };
    break;
    case 'D':
      func = [](Point& p){ p.moveDown(); };
    break;
    case 'L':
      func = [](Point& p){ p.moveLeft(); };
    break;
    case 'U':
      func = [](Point& p){ p.moveUp(); };
    break;
  }
  return func;
}

Path CreatePath(const std::string& line) {
  std::vector<std::string> tokens;

  Path path;
  Point current_point;

  boost::algorithm::split(tokens, line, boost::is_any_of(","));
  std::for_each(tokens.begin(), tokens.end(), [&path, &current_point](const std::string& token){
    auto func = GetFunction(token[0]);
    size_t count = std::stoi(token.substr(1));
    for (size_t idx = 0 ; idx < count ; idx++) {
      func(current_point);
      path.push_back(current_point);
    }
  });

  return path;
}

TEST_CASE( "Day 3 solution", "[day3]" ) {
  std::set<Point> path;
  std::string line;
  std::ifstream input_file(CURRENT_DIR "/input3.txt");
  std::vector<Path> paths;
  while(std::getline(input_file, line)) {
    paths.emplace_back(CreatePath(line));
  }

  Path intersections;

  std::vector<Points> points;
  for (const auto& path : paths)
    points.push_back(Points(path.begin(), path.end()));

  auto intersection_end = std::set_intersection(points[0].begin(), points[0].end(), points[1].begin(), points[1].end(), std::back_inserter(intersections));
  std::set<int> result;
  for (const auto& intersection : intersections)
    result.insert(std::abs(intersection.getX()) + std::abs(intersection.getY()));
  REQUIRE(*result.begin() == 1626);

  std::set<size_t> distances;
  for (const auto& intersection : intersections) {
    size_t distance = 0;
    for (const auto& path : paths) {
      distance += std::distance(path.begin(), std::find(path.begin(), path.end(), intersection)) + 1;
    }
    distances.insert(distance);
  }

  REQUIRE(*distances.begin() == 27330);
}

