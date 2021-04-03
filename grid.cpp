#include "grid.h"

Grid::Grid(const int nr_rows, const int nr_cols){
  nr_rows_ = nr_rows;
  nr_cols_ = nr_cols;
  matrix_ = std::valarray<int>(0, nr_rows*nr_cols);
}

void Grid::update_tile_weight(const int& pos) {
  if (matrix_[pos] != 9) {
    matrix_[pos]++;
    eligible_fields_[pos] = matrix_[pos];
  }
}

std::vector<int> Grid::weighted_positions_list() {
  std::vector<int> positions_list;
  for(auto& field: eligible_fields_) {
    for (int i = 1; i <= field.second; ++i){
      positions_list.push_back(field.first);
    }
  }
  return positions_list;
}

double Grid::create_random_seed(){
  std::random_device dev;
  seed_dist_t seed_distr(0, std::numeric_limits<size_t>::max());
  auto seed = seed_distr(dev);
  return seed;
}

int Grid::rnd_eligible_field(){
  std::vector<int> positions = weighted_positions_list();
  std::mt19937_64 engine(create_random_seed());
  auto rnd = bind(std::uniform_int_distribution<int>(0.0, positions.size() - 1),engine);
  return positions[rnd()];
}

void Grid::populate_field(const int& row, const int& col){
  matrix_[nr_cols_*row + col] = 9;
  eligible_fields_.erase(nr_cols_*row + col);

  if (row != 0) { update_tile_weight(nr_cols_*(row-1) + col); }
  if (col != 0) { update_tile_weight(nr_cols_*row + (col-1)); }
  if (row != nr_rows_ - 1) {update_tile_weight(nr_cols_*(row+1) + col); }
  if (col != nr_cols_ - 1) { update_tile_weight(nr_cols_*row + (col+1)); }
}

void Grid::populate_random_field(){
  auto rnd_field = rnd_eligible_field();
  populate_field(rnd_field / nr_cols_, rnd_field % nr_cols_);  
}