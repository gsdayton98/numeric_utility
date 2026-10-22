// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <numeric_utility/dijkstra.hpp>

namespace utility {
  // Relax the arc from u to v, whose cell has value d. v is Undefined when the move left the grid.
  auto Dijkstra::arc(const unsigned int u, const unsigned int v, const unsigned int d) -> void {
    if (v == Undefined || m_previous[u] == v) return;
    // 64 bits, so a sum that overflows is dropped rather than wrapping to a small distance.
    const std::uint64_t alt = static_cast<std::uint64_t>(m_distances[u]) + d;
    if (alt < m_distances[v]) {
      const auto distance = static_cast<unsigned int>(alt);
      m_distances[v] = distance;
      m_previous[v] = u;
      m_queue.emplace(v, distance);
    }
  }

  Dijkstra::Dijkstra(const Matrix& m, std::vector<unsigned int> starts, std::vector<unsigned int> targets,
                     std::vector<Direction> moves)
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

  auto Dijkstra::neighbour(const Direction direction, const unsigned int u) const -> Matrix::Cell {
    switch (direction) {
      case Direction::Left: return m_matrix.left(u);
      case Direction::Right: return m_matrix.right(u);
      case Direction::Up: return m_matrix.up(u);
      case Direction::Down: return m_matrix.down(u);
    }
    return {Undefined, Infinity};   // unreachable for a valid Direction
  }

  auto Dijkstra::operator()() -> unsigned int {
    m_distances.assign(m_matrix.size(), Infinity);
    m_previous.assign(m_matrix.size(), Undefined);
    m_queue = {};

    std::vector<bool> isTarget(m_matrix.size(), false);
    for (const auto name: m_targets) isTarget[name] = true;

    for (const auto name: m_starts) {
      m_distances[name] = m_matrix.get(name);
      m_queue.emplace(name, m_distances[name]);
    }

    while (!m_queue.empty()) {
      const auto u = m_queue.top();
      m_queue.pop();
      const auto id = u.getId();
      if (u.getDistance() > m_distances[id]) continue;   // A stale entry: a shorter path to id was found later.

      if (isTarget[id]) return u.getDistance();

      for (const auto direction: m_moves) {
        const auto [v, d] = neighbour(direction, id);
        arc(id, v, d);
      }
    }
    return Undefined;
  }
}
