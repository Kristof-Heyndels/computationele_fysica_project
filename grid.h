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
    void populate_field(const int& row, const int& col){
      matrix_(row, col) = 9;
      matrix_[nr_cols_*(row-1) + col] +=1;
      matrix_[nr_cols_*row + (col-1)] +=1;
      matrix_[nr_cols_*(row+1) + col] +=1;
      matrix_[nr_cols_*row + (col+1))] +=1;    
    }
};

std::ostream& operator<< (std::ostream& os, Grid& grid)
{
  for (int row = 0; row < grid.nr_rows(); ++row) {
    for (int col = 0; col < grid.nr_cols(); ++ col) {
      os << grid(row, col) << " ";
    }
    os << "\n";
  }

  return os;
}