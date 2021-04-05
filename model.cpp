#include "model.h"

Model::Model(Grid& g) : grid_(g) {}

int Model::ditosi(const Model::Position& pos){
  return grid_.nr_cols()*pos.row + pos.col;
}

Model::Position Model::sitodi(const int& i){
  return {i / grid_.nr_cols(), i % grid_.nr_cols()};
}

void Model::update_tile_weight(const Position& pos) {
  if (grid_(pos.row, pos.col) != 9) {
    grid_(pos.row, pos.col)++;
    eligible_fields_[ditosi(pos)] = grid_(pos.row, pos.col);
  }
}

std::vector<Model::Position> Model::weighted_positions_list() {
  std::vector<Position> positions_list;
  for(auto& field: eligible_fields_) {
    for (int i = 1; i <= field.second; ++i){
      positions_list.push_back(sitodi(field.first));
    }
  }
  return positions_list;
}

Model::Position Model::rnd_eligible_field(){
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
  eligible_fields_.erase(ditosi(pos));
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

float Model::calculate_hairiness(){
  // Needs to be float so free_edges / nr_occupied_fields_ evaluates as a float
  float free_edges {0};

  for (auto& field: eligible_fields_) {
  free_edges += field.second;
  }

  return free_edges / occupied_fields_.size();
}

Model::Position Model::find_centre_mass() {
  int centre_mass_row  {0};
  int centre_mass_col {0};
  int n = occupied_fields_.size();

  for (auto& field: occupied_fields_){
    centre_mass_row += field.row;
    centre_mass_col += field.col;
  }
  return {(centre_mass_row / n),(centre_mass_col / n)};
}