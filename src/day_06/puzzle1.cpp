#include <string>
#include <cassert>
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>

using Orbits = std::map<std::string, std::set<std::string>>;

Orbits ReadData() {
  std::string line;
  Orbits orbits;
  std::ifstream input_file(CURRENT_DIR "/input6.txt");
  std::vector<std::string> tokens;
  while(std::getline(input_file, line)) {
    tokens.clear();
    boost::algorithm::split(tokens, line, boost::is_any_of(")"));
    orbits[tokens[1]].insert(tokens[0]);
  }
  return orbits;
}

void ProcessNode(const std::string& node, int& count, Orbits& orbits) {
  for (const auto& inner_node : orbits[node]) {
    count++;
    ProcessNode(inner_node, count, orbits);
  }
}

bool ProcessNodeUntil(const std::string& node, Orbits& orbits, std::vector<std::string>& parents) {
  for (const auto& inner_node : orbits[node]) {
    if (inner_node == "COM")
      return true;
    parents.push_back(inner_node);
    auto result = ProcessNodeUntil(inner_node, orbits, parents);
    if (result)
      return true;
  }
  return false;
}

void ShortestDistanceToCommonAncestor(Orbits& orbits) {
  std::vector<std::string> parents_you;
  ProcessNodeUntil("YOU", orbits, parents_you);

  std::vector<std::string> santa_parents;
  ProcessNodeUntil("SAN", orbits, santa_parents);

  std::vector<std::set<std::string>> common_ancestors_list;
  common_ancestors_list.push_back(std::set<std::string>(parents_you.begin(), parents_you.end()));
  common_ancestors_list.push_back(std::set<std::string>(santa_parents.begin(), santa_parents.end()));

  std::vector<std::string> common_ancestors;
  std::set_intersection(common_ancestors_list[0].begin(), common_ancestors_list[0].end(),
      common_ancestors_list[1].begin(), common_ancestors_list[1].end(), std::back_inserter(common_ancestors));

  std::set<size_t> distances;
  for (const auto& node : common_ancestors) {
    size_t current_distance = 0;
    current_distance += std::distance(santa_parents.begin(),
                                      std::find(santa_parents.begin(), santa_parents.end(), node));
    current_distance += std::distance(parents_you.begin(),
                                      std::find(parents_you.begin(), parents_you.end(), node));
    distances.insert(current_distance);
  }

  std::cout << *distances.begin() << std::endl;
  assert(*distances.begin() == 547);

}

int main() {
  auto orbits = ReadData();
  std::set<std::string> planets;

  for (const auto& orbit : orbits) {
    planets.insert(orbit.first);
    for (const auto& inner_node : orbit.second) {
      planets.insert(inner_node);
    }
  }

  int count = 0;
  for (const auto& planet : planets)
    ProcessNode(planet, count, orbits);

  std::cout << count << std::endl;
  assert(count == 301100);

  ShortestDistanceToCommonAncestor(orbits);
  return 0;
}
