// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <utility>
#include <vector>
#include <numeric_utility/matrix.hpp>

using namespace utility;

BOOST_AUTO_TEST_SUITE(TestMatrix)

using Cell = std::pair<unsigned int, unsigned int>;

// 2 rows by 3 columns, cell names 0..5:
//    1 2 3
//    4 5 6
static Matrix makeMatrix() { return Matrix(2, 3, {1, 2, 3, 4, 5, 6}); }


BOOST_AUTO_TEST_CASE(test_sentinels) {
  BOOST_CHECK_EQUAL(Infinity, ~0u);
  BOOST_CHECK_EQUAL(Undefined, ~1u);
  BOOST_CHECK_NE(Infinity, Undefined);
}


BOOST_AUTO_TEST_CASE(test_size_is_the_number_of_cells) {
  BOOST_CHECK_EQUAL(makeMatrix().size(), 6U);
  BOOST_CHECK_EQUAL(Matrix(1, 1, {7}).size(), 1U);
  BOOST_CHECK_EQUAL(Matrix(4, 5, std::vector<unsigned int>(20, 0)).size(), 20U);
}


BOOST_AUTO_TEST_CASE(test_get_and_set) {
  auto matrix = makeMatrix();
  for (unsigned int n = 0; n < 6; ++n) BOOST_CHECK_EQUAL(matrix.get(n), n + 1);

  matrix.set(4, 50);
  BOOST_CHECK_EQUAL(matrix.get(4), 50U);
  BOOST_CHECK_EQUAL(matrix.get(3), 4U);     // neighbours untouched
  BOOST_CHECK_EQUAL(matrix.get(5), 6U);
}


BOOST_AUTO_TEST_CASE(test_get_out_of_range_is_infinity) {
  const auto matrix = makeMatrix();
  BOOST_CHECK_EQUAL(matrix.get(6), Infinity);
  BOOST_CHECK_EQUAL(matrix.get(1000), Infinity);
  BOOST_CHECK_EQUAL(matrix.get(Undefined), Infinity);
}


BOOST_AUTO_TEST_CASE(test_set_out_of_range_is_ignored) {
  auto matrix = makeMatrix();
  matrix.set(6, 99);
  matrix.set(Infinity, 99);
  BOOST_CHECK_EQUAL(matrix.size(), 6U);
  for (unsigned int n = 0; n < 6; ++n) BOOST_CHECK_EQUAL(matrix.get(n), n + 1);
}


BOOST_AUTO_TEST_CASE(test_the_matrix_owns_a_copy_of_its_values) {
  std::vector<unsigned int> values{1, 2, 3, 4};
  const Matrix matrix(2, 2, values);
  values[0] = 100;
  BOOST_CHECK_EQUAL(matrix.get(0), 1U);
}


BOOST_AUTO_TEST_CASE(test_name_is_row_major) {
  const auto matrix = makeMatrix();
  BOOST_CHECK_EQUAL(matrix.name(0, 0), 0U);
  BOOST_CHECK_EQUAL(matrix.name(0, 2), 2U);
  BOOST_CHECK_EQUAL(matrix.name(1, 0), 3U);
  BOOST_CHECK_EQUAL(matrix.name(1, 2), 5U);
}


BOOST_AUTO_TEST_CASE(test_name_out_of_range_is_undefined) {
  const auto matrix = makeMatrix();
  BOOST_CHECK_EQUAL(matrix.name(2, 0), Undefined);      // past the last row
  BOOST_CHECK_EQUAL(matrix.name(0, 3), Undefined);      // past the last column, which must not wrap to the next row
  BOOST_CHECK_EQUAL(matrix.name(2, 3), Undefined);
  BOOST_CHECK_EQUAL(matrix.name(Infinity, 0), Undefined);
}


BOOST_AUTO_TEST_CASE(test_position) {
  const auto matrix = makeMatrix();
  BOOST_CHECK(matrix.position(0) == Cell(0, 0));
  BOOST_CHECK(matrix.position(2) == Cell(0, 2));
  BOOST_CHECK(matrix.position(3) == Cell(1, 0));
  BOOST_CHECK(matrix.position(5) == Cell(1, 2));
}


BOOST_AUTO_TEST_CASE(test_position_beyond_the_matrix_is_undefined) {
  const auto matrix = makeMatrix();
  constexpr Cell none{Undefined, Undefined};
  BOOST_CHECK(matrix.position(6) == none);
  BOOST_CHECK(matrix.position(1000) == none);
  BOOST_CHECK(matrix.position(Infinity) == none);
}


BOOST_AUTO_TEST_CASE(test_constructor_rejects_a_bad_shape) {
  BOOST_CHECK_THROW(Matrix(0, 3, {}), std::invalid_argument);
  BOOST_CHECK_THROW(Matrix(3, 0, {}), std::invalid_argument);
  BOOST_CHECK_THROW(Matrix(0, 0, {}), std::invalid_argument);
  BOOST_CHECK_THROW(Matrix(2, 3, {1, 2, 3, 4, 5}), std::invalid_argument);        // one short
  BOOST_CHECK_THROW(Matrix(2, 3, {1, 2, 3, 4, 5, 6, 7}), std::invalid_argument);  // one over
  BOOST_CHECK_THROW(Matrix(2, 2, {1, 2, 3, 4, 5, 6}), std::invalid_argument);     // six values, but not 2 by 2
  BOOST_CHECK_NO_THROW(Matrix(2, 3, {1, 2, 3, 4, 5, 6}));
}


BOOST_AUTO_TEST_CASE(test_constructor_does_not_overflow_the_cell_count) {
  // 65536 * 65536 wraps to 0 in 32 bits, which an empty vector would then match.
  BOOST_CHECK_THROW(Matrix(65'536, 65'536, {}), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(test_moves_from_a_name_that_is_not_a_cell) {
  const auto matrix = makeMatrix();
  constexpr Cell none{Undefined, Infinity};
  for (const unsigned int u: {6u, 7u, 100u, Undefined, Infinity}) {
    BOOST_CHECK(matrix.up(u) == none);
    BOOST_CHECK(matrix.down(u) == none);
    BOOST_CHECK(matrix.left(u) == none);
    BOOST_CHECK(matrix.right(u) == none);
  }
}


BOOST_AUTO_TEST_CASE(test_name_and_position_are_inverses) {
  const Matrix matrix(3, 4, std::vector<unsigned int>(12, 1));
  for (unsigned int r = 0; r < 3; ++r) {
    for (unsigned int c = 0; c < 4; ++c) {
      BOOST_CHECK(matrix.position(matrix.name(r, c)) == Cell(r, c));
    }
  }
  for (unsigned int n = 0; n < 12; ++n) {
    const auto [r, c] = matrix.position(n);
    BOOST_CHECK_EQUAL(matrix.name(r, c), n);
  }
}


BOOST_AUTO_TEST_CASE(test_moves_inside_the_matrix_return_name_and_value) {
  // From the middle of the bottom row (name 4, value 5).
  const auto matrix = makeMatrix();
  BOOST_CHECK(matrix.left(4) == Cell(3, 4));
  BOOST_CHECK(matrix.right(4) == Cell(5, 6));
  BOOST_CHECK(matrix.up(4) == Cell(1, 2));
  // From the middle of the top row (name 1, value 2).
  BOOST_CHECK(matrix.down(1) == Cell(4, 5));
}


BOOST_AUTO_TEST_CASE(test_moves_off_the_edges_return_undefined_and_infinity) {
  const auto matrix = makeMatrix();
  constexpr Cell none{Undefined, Infinity};

  BOOST_CHECK(matrix.left(0) == none);
  BOOST_CHECK(matrix.left(3) == none);      // first column of the second row must not wrap to name 2
  BOOST_CHECK(matrix.right(2) == none);     // last column of the first row must not wrap to name 3
  BOOST_CHECK(matrix.right(5) == none);
  BOOST_CHECK(matrix.up(0) == none);
  BOOST_CHECK(matrix.up(2) == none);
  BOOST_CHECK(matrix.down(3) == none);
  BOOST_CHECK(matrix.down(5) == none);
}


BOOST_AUTO_TEST_CASE(test_corners_have_two_neighbours) {
  const Matrix matrix(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
  constexpr Cell none{Undefined, Infinity};

  BOOST_CHECK(matrix.left(0) == none);  BOOST_CHECK(matrix.up(0) == none);
  BOOST_CHECK(matrix.right(0) == Cell(1, 2));  BOOST_CHECK(matrix.down(0) == Cell(3, 4));

  BOOST_CHECK(matrix.right(2) == none); BOOST_CHECK(matrix.up(2) == none);
  BOOST_CHECK(matrix.left(2) == Cell(1, 2));   BOOST_CHECK(matrix.down(2) == Cell(5, 6));

  BOOST_CHECK(matrix.left(6) == none);  BOOST_CHECK(matrix.down(6) == none);
  BOOST_CHECK(matrix.right(6) == Cell(7, 8));  BOOST_CHECK(matrix.up(6) == Cell(3, 4));

  BOOST_CHECK(matrix.right(8) == none); BOOST_CHECK(matrix.down(8) == none);
  BOOST_CHECK(matrix.left(8) == Cell(7, 8));   BOOST_CHECK(matrix.up(8) == Cell(5, 6));
}


BOOST_AUTO_TEST_CASE(test_moves_see_values_changed_by_set) {
  auto matrix = makeMatrix();
  matrix.set(5, 60);
  BOOST_CHECK(matrix.right(4) == Cell(5, 60));
  BOOST_CHECK(matrix.down(2) == Cell(5, 60));
}


BOOST_AUTO_TEST_CASE(test_single_cell_has_no_neighbours) {
  const Matrix matrix(1, 1, {7});
  constexpr Cell none{Undefined, Infinity};
  BOOST_CHECK(matrix.left(0) == none);
  BOOST_CHECK(matrix.right(0) == none);
  BOOST_CHECK(matrix.up(0) == none);
  BOOST_CHECK(matrix.down(0) == none);
  BOOST_CHECK(matrix.position(0) == Cell(0, 0));
}


BOOST_AUTO_TEST_CASE(test_single_row_and_single_column) {
  const Matrix row(1, 4, {1, 2, 3, 4});
  constexpr Cell none{Undefined, Infinity};
  BOOST_CHECK(row.up(2) == none);
  BOOST_CHECK(row.down(2) == none);
  BOOST_CHECK(row.left(2) == Cell(1, 2));
  BOOST_CHECK(row.right(2) == Cell(3, 4));
  BOOST_CHECK(row.right(3) == none);

  const Matrix column(4, 1, {1, 2, 3, 4});
  BOOST_CHECK(column.left(2) == none);
  BOOST_CHECK(column.right(2) == none);
  BOOST_CHECK(column.up(2) == Cell(1, 2));
  BOOST_CHECK(column.down(2) == Cell(3, 4));
  BOOST_CHECK(column.down(3) == none);
}

BOOST_AUTO_TEST_SUITE_END()
