// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_MATRIX_HPP
#define NUMERIC_UTILITY_MATRIX_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>
/// @file
/// @brief A rectangular grid of values whose cells are named by their row-major index.

namespace utility {
  /**
   * A rectangular grid of values, with moves to the adjacent cells.
   *
   * Each cell has a "name", its index in row-major order: the cell at row r and column c of a grid with `cols`
   * columns is named `r * cols + c`. Moves take a name and return the neighbouring cell's name together with its
   * value, or an empty `std::optional` when the move would leave the grid. A name that is not a cell is treated the
   * same way, so no value stands in for "no cell".
   *
   * @tparam T The type of a cell's value.
   *
   * @code
   * utility::Matrix grid(2, 3, std::vector<unsigned int>{1, 2, 3,
   *                                                      4, 5, 6});
   * grid.name(1, 2);     // 5
   * grid.position(4);    // {1, 1}
   * grid.right(4);       // {5, 6}
   * grid.right(5);       // empty: column 2 is the last
   * @endcode
   */
  template <typename T = unsigned int>
  class Matrix {
  public:
    /// The type of a cell's value.
    using value_type = T;
    /// The type of a cell's name, and of row and column counts.
    using size_type = std::size_t;
    /// A cell's name and its value, as the moves return them.
    using Cell = std::pair<size_type, T>;

    /**
     * Construct a grid holding its values.
     * @param rows    The number of rows.
     * @param cols    The number of columns.
     * @param values  The `rows * cols` values, one row after the other.
     * @throws std::invalid_argument if rows or cols is 0, or values does not hold exactly `rows * cols` values.
     * @throws Exceptions from the underlying STL containers.
     */
    Matrix(const size_type rows, const size_type cols, std::vector<T> values)
      : m_rows(rows), m_cols(cols), m_values(std::move(values)) {
      if (rows == 0 || cols == 0) {
        throw std::invalid_argument("A matrix needs at least one row and one column");
      }
      // The first test keeps rows * cols from wrapping in the second.
      if (rows > m_values.size() / cols || rows * cols != m_values.size()) {
        throw std::invalid_argument("A matrix needs exactly rows * cols values");
      }
    }

    /**
     * The value of a cell.
     * @param name The cell's name.
     * @return The value, or an empty optional if there is no such cell.
     */
    [[nodiscard]] auto get(const size_type name) const -> std::optional<T> {
      if (name >= m_values.size()) return std::nullopt;
      return m_values[name];
    }

    /**
     * Change the value of a cell.
     * @param name  The cell's name.
     * @param value The new value.
     * @return true if the cell exists and was changed, false if there is no such cell.
     */
    auto set(const size_type name, T value) -> bool {
      if (name >= m_values.size()) return false;
      m_values[name] = std::move(value);
      return true;
    }

    /**
     * The name of the cell at a row and column.
     * @param r The row, counting from 0.
     * @param c The column, counting from 0.
     * @return The name `r * cols + c`, or an empty optional if either is out of range.
     */
    [[nodiscard]] auto name(const size_type r, const size_type c) const -> std::optional<size_type> {
      if (r >= m_rows || c >= m_cols) return std::nullopt;
      return r * m_cols + c;
    }

    /**
     * The row and column of a cell.
     * @param name The cell's name.
     * @return `{row, column}`, or an empty optional if there is no such cell.
     */
    [[nodiscard]] auto position(const size_type name) const -> std::optional<std::pair<size_type, size_type>> {
      if (name >= m_values.size()) return std::nullopt;
      return std::pair{name / m_cols, name % m_cols};
    }

    /**
     * The cell above.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or an empty optional in the top row or if u is not a cell.
     */
    [[nodiscard]] auto up(const size_type u) const -> std::optional<Cell> {
      if (u >= m_values.size() || u < m_cols) return std::nullopt;
      return cell(u - m_cols);
    }

    /**
     * The cell below.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or an empty optional in the bottom row or if u is not a cell.
     */
    [[nodiscard]] auto down(const size_type u) const -> std::optional<Cell> {
      // Compared as size - u so that u + cols cannot wrap.
      if (u >= m_values.size() || m_values.size() - u <= m_cols) return std::nullopt;
      return cell(u + m_cols);
    }

    /**
     * The cell to the left.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or an empty optional in the first column or if u is not a cell.
     */
    [[nodiscard]] auto left(const size_type u) const -> std::optional<Cell> {
      if (u >= m_values.size() || u % m_cols == 0) return std::nullopt;
      return cell(u - 1);
    }

    /**
     * The cell to the right.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or an empty optional in the last column or if u is not a cell.
     */
    [[nodiscard]] auto right(const size_type u) const -> std::optional<Cell> {
      if (u >= m_values.size() || (u + 1) % m_cols == 0) return std::nullopt;
      return cell(u + 1);
    }

    /// The number of cells.
    [[nodiscard]] auto size() const -> size_type { return m_values.size(); }
    /// The number of rows.
    [[nodiscard]] auto rows() const -> size_type { return m_rows; }
    /// The number of columns.
    [[nodiscard]] auto cols() const -> size_type { return m_cols; }

  private:
    [[nodiscard]] auto cell(const size_type name) const -> Cell { return {name, m_values[name]}; }

    size_type m_rows;
    size_type m_cols;
    std::vector<T> m_values;
  };
}

#endif //NUMERIC_UTILITY_MATRIX_HPP
