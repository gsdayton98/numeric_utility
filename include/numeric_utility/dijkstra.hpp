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
  /**
   * The cheapest path through a Matrix from its first cell to its last, moving up, down, left and right.
   *
   * The cost of a path is the sum of the values of the cells on it, including the first and the last.
   *
   * @code
   * utility::Matrix grid(2, 2, {1, 1,
   *                             9, 1});
   * utility::Dijkstra dijkstra(grid);
   * dijkstra();      // 3: right, then down
   * @endcode
   */
  class NUMERIC_UTILITY_API Dijkstra {
  public:
    /**
     * @param m The matrix to search. It is not copied, so it must outlive this object; a change to its values
     *          (not its shape) is seen by the next search.
     */
    explicit Dijkstra(const Matrix& m) : m_matrix(m) {}
    /// A temporary Matrix would dangle.
    explicit Dijkstra(Matrix&&) = delete;

    /**
     * Find the cost of the cheapest path from cell 0 to the last cell. Each call searches afresh, so it can be
     * repeated after changing the matrix's values.
     * @return The path cost, or Undefined if the last cell cannot be reached with a cost below Infinity (the sum
     *         of the values on every path overflows).
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

    const Matrix& m_matrix;
    std::priority_queue<Node> m_queue;
    std::vector<unsigned int> m_distances;
    std::vector<unsigned int> m_previous;
  };
}

#endif //NUMERIC_UTILITY_DIJKSTRA_HPP
