#ifndef GRID_H
#define GRID_H

#include <valarray>
#include <vector>
#include <iostream>

class Grid {
  private:
    int nr_rows_;
    int nr_cols_;
    std::valarray<int> matrix_;
    
    // double index to single index
    int ditosi(const int& row, const int& col);   

  public:    
    Grid(const int& nr_rows, const int& nr_cols);
    int nr_rows() const { return nr_rows_; }
    int nr_cols() const { return nr_cols_; }
    int& operator()(const int& row, const int& col) { return matrix_[ditosi(row, col)]; }
};

#endif