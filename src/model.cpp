#include "../include/model.h"

void Model::update_tile_weight(const Position& pos) {
  if (grid_(pos.row, pos.col) != 9) {
    grid_(pos.row, pos.col)++;
    eligible_fields_[pos] = grid_(pos.row, pos.col);
  }
}

std::vector<Position> Model::weighted_positions_list() {
  std::vector<Position> positions_list;
  for(auto& field: eligible_fields_) {
    for (int i = 1; i <= field.second; ++i){
      positions_list.push_back(field.first);
    }
  }
  return positions_list;
}

Position Model::rnd_eligible_field(){
  std::vector<Position> positions = weighted_positions_list();

  std::random_device dev;
  seed_dist_t seed_distr(0, std::numeric_limits<size_t>::max());
  auto seed = seed_distr(dev);
  std::mt19937_64 engine(seed);
  auto rnd = bind(std::uniform_int_distribution<int>(0.0, positions.size() - 1),engine);

  return positions[rnd()];
}

void Model::populate_field(const Position& pos){
  grid_(pos.row, pos.col) = 9;
  eligible_fields_.erase(pos);
  occupied_fields_.insert(pos);
  
  if (pos.row != 0) { update_tile_weight({pos.row-1, pos.col}); }
  if (pos.col != 0) { update_tile_weight({pos.row, pos.col-1}); }
  if (pos.row != grid_.nr_rows() - 1) { update_tile_weight({pos.row+1, pos.col}); }
  if (pos.col != grid_.nr_cols() - 1) { update_tile_weight({pos.row, pos.col+1}); }
}

void Model::populate_random_field(){
  auto pos = rnd_eligible_field();
  populate_field({pos.row, pos.col});  
}

float Model::hairiness(){
  // Needs to be float so free_edges / nr_occupied_fields_ evaluates as a float
  float free_edges {0};

  for (auto& field: eligible_fields_) {
    free_edges += field.second;
  }

  return free_edges / occupied_fields_.size();
}

Position Model::centre_mass() {
  int centre_mass_row  {0};
  int centre_mass_col {0};
  int n = occupied_fields_.size();

  for (auto& field: occupied_fields_){
    centre_mass_row += field.row;
    centre_mass_col += field.col;
  }
  
  return {(centre_mass_row / n),(centre_mass_col / n)};
}

float Model::inner_radius(const Position& com){
  int max_r_row = grid_.nr_rows() / 2;
  int max_r_col = grid_.nr_cols() / 2;

  for (int r = 1; r <= std::min(max_r_row, max_r_col); ++r) {
    for (int i = 1; i <= r; ++i) {
      for (int j = 1; j <= i; ++j) {
        if (grid_(com.row + i, com.col + j) != 9 || grid_(com.row - i, com.col - j) != 9) {
          return (r - 1) + 0.5;
        }
      }      
    }
  }
  return std::min(max_r_row, max_r_col) + 0.5;
}

float Model::outer_radius(const Position& com){
  float dist {0.0};
  for (auto& field: occupied_fields_){
    auto _dist = distance(com, field);
    if (_dist > dist) { dist = _dist; }
  }
  return dist + sqrt(pow_2(0.5) + pow_2(0.5));
}

float Model::density(){ 
  float area_outer_circle = M_PI * pow_2(outer_radius(centre_mass()));
  return occupied_fields_.size() / ( area_outer_circle);
}