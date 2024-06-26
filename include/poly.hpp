#pragma once

#include "catalan.hpp"
#include "global.hpp"
#include "tree.hpp"

#include <memory>
#include <string>

class Tree;

class Poly : public Catalan<Poly>, public OsOp {
public:
  Poly(Graph poly) : OsOp(_PLOT_SCRIPT, _DEFAULT_PREFIX_FILE) {
    this->poly = poly; // poly includes the root top side
    this->sides = int(poly.size()) + 2;
  }

  auto operator<=>(const Poly &rhs) const = default;

  /**
   * Constructor a catalan structure from the current base tree
   *
   * @param tree:  current base tree
   *
   * @return catalan structure of the current class
   */
  static std::unique_ptr<Poly> of(std::unique_ptr<BaseTree> tree);

  /**
   * Create a random polygon triangulation
   *
   * @param num_of_sides:   Number of sides
   *
   * @return A random polygon triangulation
   */
  static std::unique_ptr<Poly> get_random(int num_of_sides);

  /**
   * Verify if the given graph is polygon triangulation.
   *
   * @return True if it is a valid polygon triangulation, false otherwise
   */
  static bool is_valid(const Graph &graph);

  /**
   * Test poly <-> tree conversion with 3 <= num_of_sides <= TEST_MAX_SIDES_POLY
   * (default 100). There are NUM_OF_TESTS_POLY (default 50k) random test cases for
   * each `num_of_sides`.
   */
  static void test_conversion();

  /**
   * Transform to its tree representation.
   *
   * @return The root of the tree represented by the polygon
   */
  std::unique_ptr<Tree> to_tree();

  /**
   * Store the current polygon triangulation into a file
   * file by default is _DEFAULT_PREFIX_FILE
   *
   * @param file: The file to store it
   */
  void to_file(FileOp mode = FileOp::Truncate, std::string file = "");

  /**
   * Get the next polygon triangulation
   * @return The next polygon triangulation
   */
  std::unique_ptr<Poly> next();

  /**
   * Flip an edge of a polygon triangulation until the user quits
   * It takes O(n) pre-processing time and O(1) per edge flip.
   */
  void flip_and_plot();

  static constexpr std::string _PLOT_SCRIPT = "plot-poly.py";
  static constexpr std::string _DEFAULT_PREFIX_FILE = ".poly";
  static constexpr std::string _WATCHDOG_FILE = ".polylog";
  static constexpr int _NUM_OF_TESTS{20'000};
  static constexpr int _TEST_MAX_SIDES{100};

private:
  Graph poly;
  int sides;
};
