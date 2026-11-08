// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_DIJKSTRA_HPP
#define NUMERIC_UTILITY_DIJKSTRA_HPP

#include <concepts>
#include <cstddef>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>
#include <numeric_utility/matrix.hpp>
/// @file
/// @brief Dijkstra's shortest path through a Matrix.

namespace utility {
  /// A direction a path may move from one cell to the next.
  enum class Direction { Left, Right, Up, Down };

  /**
   * The cheapest path through a Matrix, moving between adjacent cells.
   *
   * The cost of a path is the sum of the values of the cells on it, including the first and the last. By default, the
   * path runs from the first cell to the last and may move in all four directions. The starts, the targets and the
   * allowed moves can be changed: the path then begins at whichever start and ends at whichever target make it
   * cheapest.
   *
   * @tparam T The matrix's value type, an unsigned integer type.
   * @tparam D The type of a path cost, an unsigned integer type at least as wide as T. It defaults to T; choose a wider
   *           type when the sum of a path could overflow T. A path whose cost would reach the largest value of D, or
   *           more, counts as not reached.
   *
   * @code
   * utility::Matrix grid(2, 2, std::vector<unsigned int>{1, 1,
   *                                                      9, 1});
   * utility::Dijkstra dijkstra(grid);
   * dijkstra();      // 3: right, then down
   *
   * // Project Euler 82: from any cell of the first column to any of the last, moving up, down and right.
   * std::vector<std::size_t> first, last;
   * for (std::size_t r = 0; r < grid.rows(); ++r) { first.push_back(*grid.name(r, 0)); last.push_back(*grid.name(r, grid.cols() - 1)); }
   * utility::Dijkstra threeWays(grid, first, last, {Direction::Up, Direction::Down, Direction::Right});
   *
   * // Costs in 64 bits over 32-bit values.
   * utility::Dijkstra<unsigned int, std::uint64_t> wide(grid);
   * @endcode
   */
  template <std::unsigned_integral T = unsigned int, std::unsigned_integral D = T>
  class Dijkstra {
    static_assert(sizeof(D) >= sizeof(T), "The cost type must be at least as wide as the value type");

  public:
    /// The type of a path cost.
    using distance_type = D;

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
    explicit Dijkstra(const Matrix<T>& m, std::vector<std::size_t> starts = {},
                      std::vector<std::size_t> targets = {},
                      std::vector<Direction> moves = {Direction::Left, Direction::Right, Direction::Up,
                                                      Direction::Down})
      : m_matrix(m), m_starts(std::move(starts)), m_targets(std::move(targets)), m_moves(std::move(moves)) {
      if (m_starts.empty()) m_starts.push_back(0);
      if (m_targets.empty()) m_targets.push_back(m.size() - 1);
      for (const auto name: m_starts) {
        if (name >= m.size()) throw std::invalid_argument("A start is not a cell of the matrix");
      }
      for (const auto name: m_targets) {
        if (name >= m.size()) throw std::invalid_argument("A target is not a cell of the matrix");
      }
    }

    /// A temporary Matrix would dangle.
    template <typename... Args>
    explicit Dijkstra(Matrix<T>&&, Args&&...) = delete;

    /**
     * Find the cost of the cheapest path from a start to a target. Each call searches afresh, so it can be repeated
     * after changing the matrix's values.
     * @return The path cost, or an empty optional if no target can be reached (there is no path with the allowed
     *         moves, or the sum of the values on every path overflows D).
     */
    auto operator()() -> std::optional<D> {
      m_distances.assign(m_matrix.size(), unreached);
      m_queue = {};

      std::vector<bool> isTarget(m_matrix.size(), false);
      for (const auto name: m_targets) isTarget[name] = true;

      for (const auto name: m_starts) {
        const auto distance = static_cast<D>(*m_matrix.get(name));
        // A start whose own value fills D cannot be told from an unreached cell.
        if (distance < m_distances[name]) {
          m_distances[name] = distance;
          m_queue.emplace(name, distance);
        }
      }

      while (!m_queue.empty()) {
        const auto u = m_queue.top();
        m_queue.pop();
        const auto id = u.getId();
        if (u.getDistance() > m_distances[id]) continue;   // A stale entry: a shorter path to id was found later.

        if (isTarget[id]) return u.getDistance();

        for (const auto direction: m_moves) {
          if (const auto v = neighbour(direction, id)) arc(id, v->first, static_cast<D>(v->second));
        }
      }
      return std::nullopt;
    }

  private:
    /// A cell and the length of the cheapest path found to it so far, ordered so that a std::priority_queue
    /// yields the nearest first.
    class Node {
    public:
      Node(const std::size_t n, const D d) : m_id(n), m_distance(d) {}

      [[nodiscard]] auto getId() const -> std::size_t { return m_id; }
      [[nodiscard]] auto getDistance() const -> D { return m_distance; }
      /// Reversed on purpose: a Node with a larger distance is "less", so the queue's top is the nearest.
      auto operator<(const Node& other) const -> bool { return m_distance > other.m_distance; }

    private:
      std::size_t m_id;
      D m_distance;
    };

    /// The internal distance of a cell not yet reached. It never leaves this class: operator() returns an optional.
    static constexpr D unreached = std::numeric_limits<D>::max();

    // Relax the arc from u to v, whose cell has value d.
    auto arc(const std::size_t u, const std::size_t v, const D d) -> void {
      // m_distances[u] < unreached, so the subtraction cannot wrap. A sum that would overflow is dropped
      // rather than wrapping to a small distance.
      if (d >= unreached - m_distances[u]) return;
      const D alt = m_distances[u] + d;
      if (alt < m_distances[v]) {
        m_distances[v] = alt;
        m_queue.emplace(v, alt);
      }
    }

    [[nodiscard]] auto neighbour(const Direction direction, const std::size_t u) const
        -> std::optional<typename Matrix<T>::Cell> {
      switch (direction) {
        case Direction::Left: return m_matrix.left(u);
        case Direction::Right: return m_matrix.right(u);
        case Direction::Up: return m_matrix.up(u);
        case Direction::Down: return m_matrix.down(u);
      }
      return std::nullopt;   // unreachable for a valid Direction
    }

    const Matrix<T>& m_matrix;
    std::vector<std::size_t> m_starts;
    std::vector<std::size_t> m_targets;
    std::vector<Direction> m_moves;
    std::priority_queue<Node> m_queue;
    std::vector<D> m_distances;
  };
}

#endif //NUMERIC_UTILITY_DIJKSTRA_HPP
