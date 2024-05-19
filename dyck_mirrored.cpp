#include "dyck_mirrored.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>

Tree *DyckPreMirrored::to_tree() {
  int id{};
  int idx{};

  std::function<Node *()> build = [&]() {
    if (idx == this->length || this->path[idx] == '1') {
      return new Node(++id);
    }

    Node *cur_node = new Node(++id, this->r);
    int pos{this->r};
    while (pos--) {
      ++idx;
      cur_node->children[pos] = build();
    }

    return cur_node;
  };

  return new Tree(build());
}

Tree *DyckPreMirrored::into_tree() {
  auto tree{this->to_tree()};
  delete this;
  return tree;
}

DyckPreMirrored *DyckPreMirrored::next() {
  // TODO
  assert(false);
}

DyckPreMirrored *DyckPreMirrored::get_random(int deg, int length) {
  if (deg <= 1) {
    std::cerr << "Degree for dyck path must be >= 2\n";
    throw std::invalid_argument("");
  }
  int num_of_internal_nodes{length / deg};
  if (length != num_of_internal_nodes * deg) {
    std::cerr << "Warning: Dyck Path length is invalid and truncated.\n";
  }
  auto random_tree{Tree::get_random(deg, num_of_internal_nodes)};
  return random_tree->into_dyck_pre_mirrored();
}

bool DyckPreMirrored::is_valid(const std::string &path) {
  int zeros = std::ranges::count(path, '0');
  int ones = std::ranges::count(path, '1');
  int sz = path.size();
  if (zeros == 0) { // must contain 0
    return false;
  }
  int down{-ones / zeros};
  int score{};
  if (zeros + ones != sz) { // only contains 1 and 0
    return false;
  }
  for (const auto c : path) { // never become < 0
    score += c == '1' ? 1 : down;
    if (score > 0) {
      return false;
    }
  }
  return score == 0;
}

void DyckPreMirrored::test_conversion() {
  std::string dots{};
  int stat_branches[DyckPreMirrored::_TEST_MAX_BRANCHES + 1];
  std::vector<std::vector<int>> stat_internal_nodes(
      DyckPreMirrored::_TEST_MAX_BRANCHES + 1);

  std::cout << "====== Conversion Test ======\n\n";
  int four_percent{DyckPreMirrored::_NUM_OF_TESTS / 25};
  std::cout << "100% remaining.";
  std::cout.flush();
  for (int i{DyckPreMirrored::_NUM_OF_TESTS}; i; --i) {
    int branches{std::uniform_int_distribution<>(
        2, DyckPreMirrored::_TEST_MAX_BRANCHES)(g_256ss)};
    int nodes{std::uniform_int_distribution<>(1, DyckPreMirrored::_TEST_MAX_EDGES /
                                                     branches)(g_256ss)};

    auto dyck_path{DyckPreMirrored::get_random(branches, branches * nodes)};
    auto tree{dyck_path->to_tree()};
    auto copied_path{tree->into_dyck_pre_mirrored()};

    ++stat_branches[branches];
    stat_internal_nodes[branches].push_back(nodes);

    if (copied_path != dyck_path) {
      std::cout << "\r";
      std::cout << "dyck path:   " << dyck_path << "\n";
      std::cout << "copied path: " << copied_path << "\n";
      assert(false);
    }

    if (int c = (i - 1) / four_percent; c * four_percent == i - 1) {
      std::cout << "\r";
      dots += ".";
      std::cout << dots + std::to_string(c * 4) + "% remaining.";
      std::cout.flush();
    }
  }
  std::cout << "\n\n";

  for (auto &v : stat_internal_nodes) {
    std::ranges::sort(v);
  }

  std::cout << "Degree\t\tInternal Nodes (Median)\t\tNumber of Tests\n";
  for (int i{2}; i <= DyckPreMirrored::_TEST_MAX_BRANCHES; ++i) {
    auto &n = stat_internal_nodes[i];
    int sz = n.size();
    int median = (sz & 1) ? n[sz >> 1] : (n[sz >> 1] + n[(sz >> 1) - 1]) >> 1;
    std::cout << i << "\t\t\t" << median << "\t\t\t" << sz << "\n";
  }

  std::cout << "\nAll " << DyckPreMirrored::_NUM_OF_TESTS << " test cases passed!\n";
}

void DyckPreMirrored::test_expected_height() {
  int num_of_samples{10'000};
  int num_of_internal_nodes{100'000};
  int ten{num_of_samples / 10};
  double tolerance{0.02};
  std::cout << "====== Sampling Distribution Test ======\n\n";
  std::cout << "Testing from degree 2 to 5 with 10k random samples and 100k "
               "internal nodes\n\n";
  for (int deg = 2; deg <= 5; ++deg) {
    double sum{};
    for (int c{}; c < num_of_samples; ++c) {
      if (c / ten * ten == c) {
        std::cout << "\r";
        std::cout << "Deg " << deg << " is " << c / ten * 10 << "% completed...";
        std::cout.flush();
      }
      auto tree{Tree::get_random(deg, num_of_internal_nodes)};
      sum += tree->height();
      tree->self_destruct();
    }
    double res{sum / num_of_samples};
    double expected{Tree::asymptote(deg, num_of_internal_nodes)};
    double error{std::abs(res - expected) / expected};
    std::cout << "\r";
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << "Result: " << res << "  |  Expected: " << expected;
    std::cout << " (Allowed Tolerance: 2.00%, got: " << error * 100 << "%)\n";
    if (error > tolerance) {
      std::cerr << "Error: Exceeded the acceptable tolerance.\n";
      assert(false);
    }
  }

  std::cout << "\n\n";
}
