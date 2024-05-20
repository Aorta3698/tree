#pragma once

#include "arcs.hpp"
#include "global.hpp"
#include "tree.hpp"
#include <memory>

class Tree;
class Arcs;

class Chords {
public:
  Chords(Graph chords) {
    this->chords = chords;
    this->points = int(chords.size()) << 1;
  }

  // loved c++20
  auto operator<=>(const Chords &rhs) const = default;

  /**
   * Test chords <-> tree conversion with 3 <= num_of_points <= TEST_MAX_SIDES_chords
   * (default 100). There are NUM_OF_TESTS_chords (default 50k) random test cases for
   * each total number of points.
   */
  static void test_conversion();

  /**
   * Get a random chords graph
   *
   * @param num_of_points:   Number of points
   *
   * @return A random chords graph
   */
  static std::unique_ptr<Chords> get_random(int num_of_points);

  /**
   * Verify if a given graph is a valid chords graph or not
   *
   * @param chords:  A chords graph
   *
   * @return True if it is a valid chords graph, false otherwise
   */
  static bool is_valid(const Graph &graph);

  /**
   * Transform the current chords to its tree representation.
   *
   * @return The tree represented by the chords graph
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Transform the current chords to its arcs graph representation.
   *
   * @return The arcs graph represented by the chords graph
   */
  std::unique_ptr<Arcs> to_arcs();

  /**
   * Get the next chords graph
   *
   * @return The root of the tree represented by the chords graph
   */
  std::unique_ptr<Chords> next();

  /**
   * Plot the current chords graph
   */
  void plot(std::string file = "");

  /**
   * Exchange 2 chords until the user quits.
   * It takes O(n) pre-processing time and O(1) per exchage.
   */
  void exchage_chords();

private:
  static constexpr std::string _PLOT_SCRIPT{"plot-chords.py"};
  static constexpr std::string _DEFAULT_PREFIX_FILE{".chords"};
  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{200};

  Graph chords;
  int points;
};