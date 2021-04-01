#include <iostream>
//#include "grid.h"
#include <valarray>
#include <vector>
#include <unordered_map>
#include <random>
#include <functional>

using namespace std;
using seed_dist_t = uniform_int_distribution<size_t>;

class Grid {
  private:
    int nr_rows_;
    int nr_cols_;
    std::valarray<int> matrix_;
    std::unordered_map<int, int> eligible_fields_;
    void update_tile_weight(const int& pos) {
      if (matrix_[pos] != 9) {
        matrix_[pos]++;
        eligible_fields_[pos] = matrix_[pos];
      }
    }
    std::vector<int> weighted_positions_list() {
      std::vector<int> positions_list;
      for(auto& field: eligible_fields_) {
        for (int i = 0; i <= field.second; ++i){
          positions_list.push_back(field.first);
        }
      }
      return positions_list;
    };
    double create_random_seed() {
      random_device dev;
      seed_dist_t seed_distr(0, numeric_limits<size_t>::max());
      auto seed = seed_distr(dev);
      return seed;
    }

  public:
    struct position{
      int row;
      int col;
    };

    Grid(const int nr_rows, const int nr_cols) :
        nr_rows_ {nr_rows},
        nr_cols_ {nr_cols},
        matrix_(0, nr_rows*nr_cols) {}
    int nr_rows() const { return nr_rows_; }
    int nr_cols() const { return nr_cols_; }
    int& operator()(const int& row, const int& col) { return matrix_[nr_cols_*row + col]; }
    int eligible_fields_count() {return eligible_fields_.size(); }
    void populate_field(const int& row, const int& col){
      matrix_[nr_cols_*row + col] = 9;
      eligible_fields_.erase(nr_cols_*row + col);

      //TODO check edge cases! There is no looparound in a matrix
      update_tile_weight(nr_cols_*(row-1) + col);
      update_tile_weight(nr_cols_*row + (col-1));
      update_tile_weight(nr_cols_*(row+1) + col);
      update_tile_weight(nr_cols_*row + (col+1));  
    }
    position rnd_eligible_field() {
      std::vector<int> positions = weighted_positions_list();

      mt19937_64 engine(create_random_seed());
      auto rnd = bind(uniform_int_distribution<int>(0.0, positions.size()),engine);

      auto random_index = rnd();
      position pos {
        positions[random_index] / nr_rows_,
        positions[random_index] % nr_cols_
      };
      return pos;
    }
};

std::ostream& operator<< (std::ostream& os, Grid& grid){
  for (int row = 0; row < grid.nr_rows(); ++row) {
    for (int col = 0; col < grid.nr_cols(); ++ col) {
      os << grid(row, col) << " ";
    }
    os << "\n";
  }

  return os;
}

void init_grid(Grid& g) {
  int centre_row = g.nr_rows()/2;
  int centre_col = g.nr_cols()/2;
  g.populate_field(centre_row, centre_col);
  // g.populate_field(centre_row - 1 , centre_col);
  // g.populate_field(centre_row - 1 , centre_col - 1);
  // g.populate_field(centre_row - 1 , centre_col - 2);
  // g.populate_field(centre_row, centre_col - 2);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "please enter grid size arguement and number of time steps" << "\n";
    return 2;
  }

  int n = std::stoi(argv[1]);
  int t = std::stoi(argv[2]);

  Grid grid(n,n);
  init_grid(grid);

  //TODO: check why diagonal fields are counted as neighbours
  for (int i = 1; i != t; ++i){
    grid.populate_field(grid.rnd_eligible_field().row,  grid.rnd_eligible_field().col);
  }


  std::cout << grid << "\n";

  return 0;
}