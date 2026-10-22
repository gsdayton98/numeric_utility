// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_DIJKSTRA_HPP
#define NUMERIC_UTILITY_DIJKSTRA_HPP

#include <queue>
#include <vector>
#include <numeric_utility/matrix.hpp>
#include <numeric_utility/numeric_utility_export.h>
/// @file
/// @brief Dijkstra's shortest path through a Matrix.

namespace utility {
  /// A direction a path may move from one cell to the next.
  enum class Direction { Left, Right, Up, Down };

  /**
   * The cheapest path through a Matrix, moving between adjacent cells.
   *
   * The cost of a path is the sum of the values of the cells on it, including the first and the last. By default the
   * path runs from the first cell to the last and may move in all four directions. The starts, the targets and the
   * allowed moves can be changed: the path then begins at whichever start and ends at whichever target make it
   * cheapest.
   *
   * @code
   * utility::Matrix grid(2, 2, {1, 1,
   *                             9, 1});
   * utility::Dijkstra dijkstra(grid);
   * dijkstra();      // 3: right, then down
   *
   * // Project Euler 82: from any cell of the first column to any of the last, moving up, down and right.
   * std::vector<unsigned int> first, last;
   * for (unsigned int r = 0; r < rows; ++r) { first.push_back(grid.name(r, 0)); last.push_back(grid.name(r, cols - 1)); }
   * utility::Dijkstra threeWays(grid, first, last, {Direction::Up, Direction::Down, Direction::Right});
   * @endcode
   */
  class NUMERIC_UTILITY_API Dijkstra {
  public:
    /**
     * @param m       The matrix to search. It is not copied, so it must outlive this object; a change to its values
     *                (not its shape) is seen by the next search.
     * @param starts  The names of the cells a path may begin at. Empty means just the first cell.
     * @param targets The names of the cells a path may end at. Empty means just the last cell.
     * @param moves   The directions a path may move in. Empty means the path cannot move, so it is a single cell
     *                that is both a start and a target.
     * @throws std::invalid_argument if a start or a target is not a cell of m.
     * @throws Exceptions from the underlying STL containers.
     */
    explicit Dijkstra(const Matrix& m, std::vector<unsigned int> starts = {}, std::vector<unsigned int> targets = {},
                      std::vector<Direction> moves = {Direction::Left, Direction::Right, Direction::Up,
                                                      Direction::Down});
    /// A temporary Matrix would dangle.
    template <typename... Args>
    explicit Dijkstra(Matrix&&, Args&&...) = delete;

    /**
     * Find the cost of the cheapest path from a start to a target. Each call searches afresh, so it can be repeated
     * after changing the matrix's values.
     * @return The path cost, or Undefined if no target can be reached with a cost below Infinity (there is no path
     *         with the allowed moves, or the sum of the values on every path overflows).
     */
    auto operator()() -> unsigned int;

  private:
    /// A cell and the length of the cheapest path found to it so far, ordered so that a std::priority_queue
    /// yields the nearest first.
    class Node {
    public:
      Node(const unsigned int n, const unsigned int d) : m_id(n), m_distance(d) {}

      [[nodiscard]] auto getId() const -> unsigned int { return m_id; }
      [[nodiscard]] auto getDistance() const -> unsigned int { return m_distance; }
      /// Reversed on purpose: a Node with a larger distance is "less", so the queue's top is the nearest.
      auto operator<(const Node& other) const -> bool { return m_distance > other.m_distance; }

    private:
      unsigned int m_id;
      unsigned int m_distance;
    };

    auto arc(unsigned int u, unsigned int v, unsigned int d) -> void;
    [[nodiscard]] auto neighbour(Direction direction, unsigned int u) const -> Matrix::Cell;

    const Matrix& m_matrix;
    std::vector<unsigned int> m_starts;
    std::vector<unsigned int> m_targets;
    std::vector<Direction> m_moves;
    std::priority_queue<Node> m_queue;
    std::vector<unsigned int> m_distances;
    std::vector<unsigned int> m_previous;
  };
}

#endif //NUMERIC_UTILITY_DIJKSTRA_HPP
