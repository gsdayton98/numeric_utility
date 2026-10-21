// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#include <cstdint>
#include <stdexcept>
#include <numeric_utility/matrix.hpp>

namespace utility {
  Matrix::Matrix(const unsigned int rows, const unsigned int cols, const std::vector<unsigned int>& values)
    : m_rows(rows), m_cols(cols), m_values(values) {
    if (rows == 0 || cols == 0) {
      throw std::invalid_argument("A matrix needs at least one row and one column");
    }
    // The product of two 32-bit numbers fits in 64 bits.
    if (static_cast<std::uint64_t>(rows) * cols != values.size()) {
      throw std::invalid_argument("A matrix needs exactly rows * cols values");
    }
  }

  auto Matrix::name(const unsigned int r, const unsigned int c) const -> unsigned int {
    return r < m_rows && c < m_cols ? r * m_cols + c : Undefined;
  }

  auto Matrix::position(const unsigned int name) const -> std::pair<unsigned int, unsigned int> {
    if (name >= m_values.size()) return {Undefined, Undefined};
    return {name / m_cols, name % m_cols};
  }

  auto Matrix::up(const unsigned int u) const -> Cell {
    if (u >= m_values.size() || u < m_cols) return {Undefined, Infinity};
    const auto v = u - m_cols;
    return {v, m_values[v]};
  }

  auto Matrix::down(const unsigned int u) const -> Cell {
    // Compared as size - u so that u + cols cannot wrap.
    if (u >= m_values.size() || m_values.size() - u <= m_cols) return {Undefined, Infinity};
    const auto v = u + m_cols;
    return {v, m_values[v]};
  }

  auto Matrix::left(const unsigned int u) const -> Cell {
    if (u >= m_values.size() || u % m_cols == 0) return {Undefined, Infinity};
    const auto v = u - 1;
    return {v, m_values[v]};
  }

  auto Matrix::right(const unsigned int u) const -> Cell {
    if (u >= m_values.size() || (u + 1) % m_cols == 0) return {Undefined, Infinity};
    const auto v = u + 1;
    return {v, m_values[v]};
  }
}
