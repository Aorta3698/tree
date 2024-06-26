#include "util.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numbers>
#include <sstream>
#include <unistd.h>

void Util::setup_comb() {
  mint n{1};
  fact[0] = n;
  for (int i{1}; i <= 10'000; ++i) {
    n *= i;
    fact[i] = n;
  }
}

mint Util::combinations(int n, int k) {
  if (n > 10'000) {
    std::cerr << "only support at most 10,000 nodes now\n\n";
    throw std::invalid_argument("");
  }
  return fact[n] / fact[k] / fact[n - k];
}

std::vector<std::string> Util::split_string(std::string s, char delimiter) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  std::stringstream ss{s};
  std::vector<std::string> ans;
  std::string cur_str{};
  while (std::getline(ss, cur_str, delimiter)) {
    ans.push_back(Util::strip(cur_str));
  }
  return ans;
}

int Util::get_num(std::string s) {
  size_t processed_count{};
  int num{std::stoi(s, &processed_count)};
  if (processed_count != s.size() || num < 1) {
    throw std::invalid_argument("num processing failure");
  }
  return num;
}

std::string &Util::strip(std::string &str) {
  str.erase(str.find_last_not_of(' ') + 1);
  str.erase(0, str.find_first_not_of(' '));
  return str;
}

int Util::get_catalan(int num) {
  auto cata = std::to_array({1,       1,        2,         5,         14,
                             42,      132,      429,       1430,      4862,
                             16796,   58786,    208012,    742900,    2674440,
                             9694845, 35357670, 129644790, 477638700, 1767263190});
  return cata.at(num);
}

std::array<double, 2> Util::rotate(double x, double y, double deg) {
  double radian{deg * (std::numbers::pi / 180)};

  double nx{x * std::cos(radian) - y * std::sin(radian)};
  double ny{x * std::sin(radian) + y * std::cos(radian)};

  return {nx, ny};
}
