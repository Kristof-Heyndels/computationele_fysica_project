#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <set>
#include <random>
#include <functional>
#include "grid.h"

using seed_dist_t = std::uniform_int_distribution<size_t>;

class Model{
  public:
    struct Position{
      int row;
      int col;
    };

  private:
    Grid grid_;
    //TODO would like to make these unordered_map<Position, int> and set<Position>
    std::unordered_map<int, int> eligible_fields_;
    std::set<int> occupied_fields_;

    // double index to single index
    int ditosi(const Model::Position& pos);
    // single index to double index
    Model::Position sitodi(const int& i);
    void update_tile_weight(const Position& pos);
    std::vector<Position> weighted_positions_list();
    Position rnd_eligible_field();

  public:
    Model(Grid& g);
    Grid grid() {return grid_; }
    int eligible_fields_count() {return eligible_fields_.size(); }
    void populate_field(const Position& pos);
    void populate_random_field();
    float calculate_hairiness();
    Position find_centre_mass();
};

#endif