// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-;
// Copyright 2025 Glen S. Dayton. Rights reserved according to terms of included license.
#ifndef NUMERIC_UTILITY_MATRIX_HPP
#define NUMERIC_UTILITY_MATRIX_HPP

#include <utility>
#include <vector>
#include <numeric_utility/numeric_utility_export.h>
/// @file
/// @brief A rectangular grid of unsigned values whose cells are named by their row-major index.

namespace utility {
  /// The distance to a cell that has not been reached, and the value of a cell that does not exist.
  inline constexpr unsigned int Infinity = ~0u;
  /// The name of a cell that does not exist.
  inline constexpr unsigned int Undefined = ~1u;

  /**
   * A rectangular grid of unsigned values, with moves to the adjacent cells.
   *
   * Each cell has a "name", its index in row-major order: the cell at row r and column c of a grid with `cols`
   * columns is named `r * cols + c`. Moves take a name and return the neighbouring cell's name together with its
   * value, or `{Undefined, Infinity}` when the move would leave the grid.
   *
   * @code
   * utility::Matrix grid(2, 3, {1, 2, 3,
   *                             4, 5, 6});
   * grid.name(1, 2);     // 5
   * grid.position(4);    // {1, 1}
   * grid.right(4);       // {5, 6}
   * grid.right(5);       // {Undefined, Infinity}: column 2 is the last
   * @endcode
   */
  class NUMERIC_UTILITY_API Matrix {
  public:
    /// A cell's name and its value, as the moves return them.
    using Cell = std::pair<unsigned int, unsigned int>;

    /**
     * Construct a grid holding a copy of its values.
     * @param rows    The number of rows.
     * @param cols    The number of columns.
     * @param values  The `rows * cols` values, one row after the other.
     * @throws std::invalid_argument if rows or cols is 0, or values does not hold exactly `rows * cols` values.
     * @throws Exceptions from the underlying STL containers.
     */
    Matrix(unsigned int rows, unsigned int cols, const std::vector<unsigned int>& values);

    /**
     * The value of a cell.
     * @param name The cell's name.
     * @return The value, or Infinity if there is no such cell.
     */
    [[nodiscard]] auto get(unsigned int name) const -> unsigned int {
      return name < m_values.size() ? m_values[name] : Infinity;
    }

    /**
     * Change the value of a cell. Does nothing if there is no such cell.
     * @param name  The cell's name.
     * @param value The new value.
     */
    auto set(unsigned int name, unsigned int value) -> void {
      if (name < m_values.size()) m_values[name] = value;
    }

    /**
     * The name of the cell at a row and column.
     * @param r The row, counting from 0.
     * @param c The column, counting from 0.
     * @return The name `r * cols + c`, or Undefined if either is out of range.
     */
    [[nodiscard]] auto name(unsigned int r, unsigned int c) const -> unsigned int;

    /**
     * The row and column of a cell.
     * @param name The cell's name.
     * @return `{row, column}`, or `{Undefined, Undefined}` if there is no such cell.
     */
    [[nodiscard]] auto position(unsigned int name) const -> std::pair<unsigned int, unsigned int>;

    /**
     * The cell above.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or `{Undefined, Infinity}` in the top row or if u is not a cell.
     */
    [[nodiscard]] auto up(unsigned int u) const -> Cell;

    /**
     * The cell below.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or `{Undefined, Infinity}` in the bottom row or if u is not a cell.
     */
    [[nodiscard]] auto down(unsigned int u) const -> Cell;

    /**
     * The cell to the left.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or `{Undefined, Infinity}` in the first column or if u is not a cell.
     */
    [[nodiscard]] auto left(unsigned int u) const -> Cell;

    /**
     * The cell to the right.
     * @param u The name of the starting cell.
     * @return The neighbour's name and value, or `{Undefined, Infinity}` in the last column or if u is not a cell.
     */
    [[nodiscard]] auto right(unsigned int u) const -> Cell;

    /**
     * The number of cells.
     * @return The number of rows times the number of columns.
     */
    [[nodiscard]] auto size() const -> unsigned int { return static_cast<unsigned int>(m_values.size()); }

  private:
    unsigned int m_rows;
    unsigned int m_cols;
    std::vector<unsigned int> m_values;
  };
}

#endif //NUMERIC_UTILITY_MATRIX_HPP
