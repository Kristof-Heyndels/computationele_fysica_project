#ifndef GRID_H
#define GRID_H

#include <valarray>

class Grid {
  private:
    int nr_rows_;
    int nr_cols_;
    std::valarray<int> matrix_;

  public:          
    Grid(const int nr_rows, const int nr_cols) :
      nr_rows_ {nr_rows},
      nr_cols_ {nr_cols},
      matrix_(0, nr_rows*nr_cols) {}
    int nr_rows() const { return nr_rows_; }
    int nr_cols() const { return nr_cols_; }
    int& operator()(const int row, const int col) { return matrix_[nr_cols_*row + col]; }
    std::ostream& operator<< (std::ostream& os);
};

#endif