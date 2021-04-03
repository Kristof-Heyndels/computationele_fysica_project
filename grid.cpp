#include "grid.h"
#include <iostream>

Grid::Grid(const int nr_rows, const int nr_cols){
  nr_rows_ = nr_rows;
  nr_cols_ = nr_cols;
  matrix_ = std::valarray<int>(0, nr_rows*nr_cols);
  occupied_fields_ = {};
}

Grid::Position Grid::sitodi(const int& index){
  return {index / nr_cols_, index % nr_cols_};
}

int Grid::ditosi(const int& row, const int& col){
  return nr_cols_*row + col;
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

Grid::Carthesian_Position Grid::transform_position_to_carthesian(const Grid::Position& pos){
  float x_offset = nr_cols_ / 2;
  float y_offset = nr_rows_ / 2;
  return {pos.col - x_offset, -1 * (pos.row - y_offset)};
}


int Grid::rnd_eligible_field(){
  std::vector<int> positions = weighted_positions_list();
  std::mt19937_64 engine(create_random_seed());
  auto rnd = bind(std::uniform_int_distribution<int>(0.0, positions.size() - 1),engine);
  return positions[rnd()];
}

void Grid::populate_field(const int& row, const int& col){
  matrix_[ditosi(row, col)] = 9;
  eligible_fields_.erase(ditosi(row, col));
  occupied_fields_.insert(ditosi(row, col));

  if (row != 0) { update_tile_weight(ditosi(row-1, col)); }
  if (col != 0) { update_tile_weight(ditosi(row, col-1)); }
  if (row != nr_rows_ - 1) {update_tile_weight(ditosi(row+1, col)); }
  if (col != nr_cols_ - 1) { update_tile_weight(ditosi(row, col+1)); }
}

void Grid::populate_random_field(){
  auto pos = sitodi(rnd_eligible_field());
  populate_field(pos.row, pos.col);  
}

float Grid::calculate_hairiness(){
  // Needs to be float so free_edges / nr_occupied_fields_ evaluates as a float
  float free_edges {0};

  for (auto& field: eligible_fields_) {
  free_edges += field.second;
  }

  return free_edges / occupied_fields_.size();
}

Grid::Carthesian_Position Grid::find_centre_mass() {
  float centre_mass_x  {0};
  float centre_mass_y {0};
  int n = occupied_fields_.size();

  for (auto& field: occupied_fields_){
    Carthesian_Position c_pos = transform_position_to_carthesian(sitodi(field));
    centre_mass_x += c_pos.x;
    centre_mass_y += c_pos.y;
  }
  return {(centre_mass_x / n),(centre_mass_y / n)};
}