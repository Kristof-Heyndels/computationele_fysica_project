#include "grid.h"

Grid::Grid(const int& nr_rows, const int& nr_cols){
  nr_rows_ = nr_rows;
  nr_cols_ = nr_cols;
  matrix_ = std::valarray<int>(0, nr_rows*nr_cols);
}

int Grid::ditosi(const int& row, const int& col){
  return nr_cols_*row + col;
}

std::ostream& operator<< (std::ostream& os, Grid& grid){
  for (int row = 0; row < grid.nr_rows(); ++row) {
    for (int col = 0; col < grid.nr_cols(); ++ col) {
      os << grid(row, col) << " ";
    }
    os << "\n";
  }

  return os;
}