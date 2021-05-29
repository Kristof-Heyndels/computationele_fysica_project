#include "../include/model.h"

void Model::update_field_weight(const Position& pos) {
  if (grid_(pos.row, pos.col) != 9) {
    grid_(pos.row, pos.col)++;
    eligible_fields_[pos] = grid_(pos.row, pos.col);
  }
}

// Create a vector filled with eligible field positions. Each field position is 
// stored n times, n being the field weight.
std::vector<Position> Model::weighted_positions_list() {
  std::vector<Position> positions_list;
  for(auto& field: eligible_fields_) {
    for (int i = 1; i <= field.second; ++i){
      positions_list.push_back(field.first);
    }
  }
  return positions_list;
}

// Returns a random field position by selecting a random index.
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
  
  // Checking boundaries before adjusting weight of each neighbouring field
  if (pos.row != 0) {update_field_weight({pos.row-1, pos.col});}
  if (pos.col != 0) {update_field_weight({pos.row, pos.col-1});}
  if (pos.row != grid_.nr_rows() - 1) {update_field_weight({pos.row+1, pos.col});}
  if (pos.col != grid_.nr_cols() - 1) {update_field_weight({pos.row, pos.col+1});}

  invalidate_buffers();
}

void Model::populate_random_field(){
  auto pos = rnd_eligible_field();
  populate_field({pos.row, pos.col});  
}

float Model::hairiness(){
  
  if (g_hair_ != -1) { return g_hair_ ; }

  // Needs to be float so free_edges / nr_occupied_fields_ evaluates as a float
  float free_edges {0};

  for (auto& field: eligible_fields_) {
    free_edges += field.second;
  }

  g_hair_ = free_edges / occupied_fields_.size();
  return g_hair_;
}

Position Model::centre_mass() {
  if (g_com_.row != -1 && g_com_.col != -1) { return g_com_ ; }

  int centre_mass_row  {0};
  int centre_mass_col {0};
  int n = occupied_fields_.size();

  for (auto& field: occupied_fields_){
    centre_mass_row += field.row;
    centre_mass_col += field.col;
  }
  
  g_com_ = {(centre_mass_row / n),(centre_mass_col / n)};
  return g_com_;
}

std::tuple<float, float> Model::inner_outer_radius(const Position& com){

  if(g_inner_ != -1 && g_outer_ != -1) { return {g_inner_,g_outer_}; }

  float nearest_eligible_field = std::max(grid_.nr_cols(), grid_.nr_rows());
  float furthest_eligible_field {0.0};

  for (auto& field: eligible_fields_){
    auto _dist = distance(com, field.first);
    if (_dist < nearest_eligible_field) { nearest_eligible_field = _dist; }
    if (_dist > furthest_eligible_field) { furthest_eligible_field = _dist; }
  }

  g_inner_ = nearest_eligible_field;
  g_outer_ = furthest_eligible_field;
  return {g_inner_, g_outer_};
}

float Model::density(){ 

  if (g_density_ != -1) { return g_density_; }

  float area_outer_circle = M_PI * pow_2(outer_radius(centre_mass()));
  return occupied_fields_.size() / ( area_outer_circle);
}

void Model::invalidate_buffers(){
  g_hair_ = -1;
  g_com_ = {-1,-1};
  g_inner_ = -1;
  g_outer_ = -1;
  g_density_ = -1;
}