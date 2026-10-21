// -*- mode:C++; c-basic-offset:2; indent-tabs-mode:nil -*-
// Copyright 2026 Glen S. Dayton. Rights reserved according to terms of included license.
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <random>
#include <utility>
#include <vector>
#include <numeric_utility/dijkstra.hpp>
#include <numeric_utility/matrix.hpp>

using namespace utility;

BOOST_AUTO_TEST_SUITE(TestDijkstra)

using Cell = std::pair<unsigned int, unsigned int>;


BOOST_AUTO_TEST_CASE(test_single_cell) {
  Matrix matrix(1, 1, {7});
  Dijkstra dijkstra(matrix);
  BOOST_CHECK_EQUAL(dijkstra(), 7U);
}


BOOST_AUTO_TEST_CASE(test_single_row_and_single_column) {
  Matrix row(1, 4, {1, 2, 3, 4});
  BOOST_CHECK_EQUAL(Dijkstra(row)(), 10U);

  Matrix column(4, 1, {1, 2, 3, 4});
  BOOST_CHECK_EQUAL(Dijkstra(column)(), 10U);
}


BOOST_AUTO_TEST_CASE(test_the_path_sum_counts_both_end_cells) {
  Matrix matrix(1, 2, {5, 9});
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 14U);
}


BOOST_AUTO_TEST_CASE(test_two_by_two_takes_the_cheaper_way_around) {
  Matrix viaTop(2, 2, {1, 1, 9, 1});         // 1 + 1 + 1
  BOOST_CHECK_EQUAL(Dijkstra(viaTop)(), 3U);

  Matrix viaLeft(2, 2, {1, 9, 1, 1});        // 1 + 1 + 1
  BOOST_CHECK_EQUAL(Dijkstra(viaLeft)(), 3U);
}


BOOST_AUTO_TEST_CASE(test_statement_example) {
  // The 5 by 5 matrix from the Project Euler 83 statement.
  Matrix matrix(5, 5, {
      131, 673, 234, 103, 18,
      201, 96, 342, 965, 150,
      630, 803, 746, 422, 111,
      537, 699, 497, 121, 956,
      805, 732, 524, 37, 331
  });
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 2297U);
}


BOOST_AUTO_TEST_CASE(test_the_path_may_move_up_and_left) {
  // The cheapest path winds through the 1s, going up and left. Moving only right and down costs 15.
  Matrix matrix(3, 5, {
      1, 9, 1, 1, 1,
      1, 9, 1, 9, 1,
      1, 1, 1, 9, 1
  });
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 11U);

  // A spiral that must go left as well as up to reach the end.
  Matrix spiral(3, 3, {
      1, 1, 1,
      9, 9, 1,
      1, 1, 1
  });
  BOOST_CHECK_EQUAL(Dijkstra(spiral)(), 5U);
}


BOOST_AUTO_TEST_CASE(test_uniform_matrix_takes_the_shortest_number_of_cells) {
  // Every path has at least rows + cols - 1 cells.
  Matrix matrix(6, 9, std::vector<unsigned int>(54, 3));
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 3U * (6 + 9 - 1));
}


BOOST_AUTO_TEST_CASE(test_uses_values_changed_by_set) {
  Matrix matrix(2, 2, {1, 1, 9, 1});
  matrix.set(1, 50);
  matrix.set(2, 2);
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 4U);          // 1 + 2 + 1 now beats 1 + 50 + 1
}


BOOST_AUTO_TEST_CASE(test_repeated_calls_give_the_same_answer) {
  Matrix matrix(3, 5, {
      1, 9, 1, 1, 1,
      1, 9, 1, 9, 1,
      1, 1, 1, 9, 1
  });
  Dijkstra dijkstra(matrix);
  BOOST_CHECK_EQUAL(dijkstra(), 11U);
  BOOST_CHECK_EQUAL(dijkstra(), 11U);
  BOOST_CHECK_EQUAL(dijkstra(), 11U);
}


BOOST_AUTO_TEST_CASE(test_a_second_call_sees_values_changed_since_the_first) {
  Matrix matrix(2, 2, {1, 1, 9, 1});
  Dijkstra dijkstra(matrix);
  BOOST_CHECK_EQUAL(dijkstra(), 3U);
  matrix.set(1, 50);
  matrix.set(2, 2);
  BOOST_CHECK_EQUAL(dijkstra(), 4U);
  matrix.set(1, 1);
  BOOST_CHECK_EQUAL(dijkstra(), 3U);
}


BOOST_AUTO_TEST_CASE(test_a_sum_that_overflows_is_not_reached) {
  // Every path costs more than Infinity, which a wrapped sum would hide as a small number.
  constexpr unsigned int big = 3'000'000'000U;
  Matrix matrix(1, 3, {big, big, big});
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), Undefined);
}


BOOST_AUTO_TEST_CASE(test_a_large_sum_below_infinity_is_exact) {
  constexpr unsigned int big = 2'000'000'000U;
  Matrix matrix(1, 2, {big, big});
  BOOST_CHECK_EQUAL(Dijkstra(matrix)(), 4'000'000'000U);
}


BOOST_AUTO_TEST_CASE(test_does_not_change_the_matrix) {
  Matrix matrix(2, 3, {1, 2, 3, 4, 5, 6});
  Dijkstra dijkstra(matrix);
  dijkstra();
  for (unsigned int n = 0; n < 6; ++n) BOOST_CHECK_EQUAL(matrix.get(n), n + 1);
}


BOOST_AUTO_TEST_CASE(test_agrees_with_repeated_relaxation) {
  // An independent method: relax every cell from its four neighbours until nothing improves. Random matrices make the
  // cheapest path wander up and left as well as down and right.
  std::mt19937 generator(83);
  for (int trial = 0; trial < 50; ++trial) {
    const unsigned int rows = std::uniform_int_distribution<unsigned int>(1, 8)(generator);
    const unsigned int cols = std::uniform_int_distribution<unsigned int>(1, 8)(generator);
    std::vector<unsigned int> values(rows * cols);
    for (auto& value : values) value = std::uniform_int_distribution<unsigned int>(1, 999)(generator);

    std::vector<unsigned int> best(rows * cols, 1'000'000'000U);
    best[0] = values[0];
    for (bool changed = true; changed;) {
      changed = false;
      for (unsigned int r = 0; r < rows; ++r) {
        for (unsigned int c = 0; c < cols; ++c) {
          const Cell neighbours[] = {{r, c + 1}, {r + 1, c}, {r, c - 1}, {r - 1, c}};   // c - 1 wraps; excluded below
          for (const auto [rr, cc] : neighbours) {
            if (rr >= rows || cc >= cols) continue;
            if (const auto candidate = best[rr * cols + cc] + values[r * cols + c]; candidate < best[r * cols + c]) {
              best[r * cols + c] = candidate;
              changed = true;
            }
          }
        }
      }
    }

    Matrix matrix(rows, cols, values);
    BOOST_CHECK_EQUAL(Dijkstra(matrix)(), best.back());
  }
}

BOOST_AUTO_TEST_SUITE_END()
