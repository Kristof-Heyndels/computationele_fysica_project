#include "../include/grid.h"
#include <iostream>

std::ostream& operator<< (std::ostream& os, Grid& grid){
  for (int row = 0; row < grid.nr_rows(); ++row) {
    for (int col = 0; col < grid.nr_cols(); ++ col) {
      //TODO can we check if console is compatible?
      if (grid(row,col) == 9) { os << "\033[1;34m" << grid(row, col) << "\033[0m "; }
      else if (grid(row,col) == 1) { os << "\033[1;31m" << grid(row, col) << "\033[0m "; }
      else if (grid(row,col) == 2) { os << "\033[1;33m" << grid(row, col) << "\033[0m "; }
      else if (grid(row,col) == 3) { os << "\033[1;32m" << grid(row, col) << "\033[0m "; }
      else if (grid(row,col) == 4) { os << "\033[1;36m" << grid(row, col) << "\033[0m "; }
      else { os << grid(row, col) << " " ; }
    }
    os << "\n";
  }

  return os;
}