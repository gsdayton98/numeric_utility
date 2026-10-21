// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include <cstdint>
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

  auto Dijkstra::operator()() -> unsigned int {
    m_distances.assign(m_matrix.size(), Infinity);
    m_previous.assign(m_matrix.size(), Undefined);
    m_queue = {};

    m_distances[0] = m_matrix.get(0);
    m_queue.emplace(0, m_distances[0]);

    while (!m_queue.empty()) {
      const auto u = m_queue.top();
      m_queue.pop();
      const auto id = u.getId();
      if (u.getDistance() > m_distances[id]) continue;   // A stale entry: a shorter path to id was found later.

      if (id == m_matrix.size() - 1) return u.getDistance();

      for (const auto& [v, d] : {m_matrix.left(id), m_matrix.right(id), m_matrix.up(id), m_matrix.down(id)}) {
        arc(id, v, d);
      }
    }
    return Undefined;
  }
}
